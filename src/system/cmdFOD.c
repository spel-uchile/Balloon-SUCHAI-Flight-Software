/*                                 SUCHAI
 *                      NANOSATELLITE FLIGHT SOFTWARE
 *
 *      Copyright 2018, Carlos Gonzalez Cortes, carlgonz@ug.uchile.cl
 *      Copyright 2018, Tomas Opazo Toro, tomas.opazo.t@gmail.com
 *      Copyright 2018, Matias Ramirez Martinez, nicoram.mt@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cmdFOD.h"

static const char* tag = "cmdFOD";
uint16_t clk_div = BCM2835_I2C_CLOCK_DIVIDER_148;

void cmd_fod_init(void)
{
    #ifdef LINUX
    cmd_add("fod_help", help, "", 0);
    cmd_add("fod_deploy", deploy_femtosats, "", 0);
    cmd_add("fod_get_status", get_status, "", 0);
    cmd_add("fod_get_femtosat_data", get_femtosat_data, "", 0);
    cmd_add("fod_set_on_time", set_on_time, "%d", 1);
    cmd_add("fod_send_beacon", send_beacon, "", 0);
    cmd_add("fod_update_data", update_data, "%d %d %d %d %s %s %s", 7);
    cmd_add("fod_enable_low_power", enable_low_power_mode, "", 0);
    cmd_add("fod_disable_low_power", disable_low_power_mode, "", 0);
    cmd_add("fod_get_version", get_version, "", 0);
    #endif

    #ifdef NANOMIND
        //TO DO
    #endif
}

int help(char *fmt, char *params, int nparams) {
    i2c_write((char *)GET_VERSION, NULL);
    char ver[4];
    i2c_read(ver);
    i2c_write((char *)FOD_GET_STATUS, NULL);
    char status[1];
    i2c_read(status);
    LOGI(tag, "Payload: FOD\nVersion: %s\nStatus: %s", &ver, &status);
    return CMD_OK;
}

int deploy_femtosats(char *fmt, char *params, int nparams) {
    LOGI(tag, "Deploying femto-satellites");
    if (i2c_write((char *)DEPLOY_FEMTOSATS, NULL)) {
        i2c_write((char *)FOD_GET_STATUS, NULL);
        char res[1];
	i2c_read(res);
	if (res == '1')
	    LOGI(tag, "Released");
	else
	    LOGI(tag, "Not released")
	return CMD_OK;
    }
    return CMD_FAIL;
}

int get_status(char *fmt, char *params, int nparams) {
    LOGI(tag, "Getting FOD's status");
    if (i2c_write((char *)FOD_GET_STATUS, NULL))
	return CMD_OK;
    return CMD_FAIL;
}

int get_femtosat_data(char *fmt, char *params, char nparams) {
    LOGI(tag, "Getting femto-satellites' data");
    if (i2c_write((char *)GET_FEMTOSAT_DATA, NULL))
	return CMD_OK;
    return CMD_FAIL;
}

int set_on_time(char *fmt, char *params, char nparams) {
    if (params == NULL) {
	LOGE(tag, "NULL params!");
	return CMD_FAIL;
    }
    char on_time[5];
    
    if (sscanf(params, fmt, &on_time) == nparams) {
	LOGI(tag, "Setting on time to %d", on_time);
	i2c_write((char *)SET_ON_TIME, params);
	return CMD_OK;
    }
    else {
	LOGE(tag, "Invalid params!");
	return CMD_FAIL;
    }
}

int send_beacon(char *fmt, char *params, char nparams) {
    LOGI(tag, "Sending beacon");
    if (i2c_write((char *)SEND_BEACON, params))
	return CMD_OK;
    return CMD_FAIL;
}

int update_data(char *fmt, char *params, char nparams) {
    if (params == NULL) {
	LOGE(tag, "NULL params!");
	return CMD_FAIL;
    }
    int hour, min, sec, sats;
    String lat, lng, alt;

    if (sscanf(params, fmt, &hour, &min, &sec, &sats, &lat, &lng, &alt) == nparams) {
	LOGI(tag, "Updating FOD's data:\nHH:MM:SS: %d:%d:%d\nSats: %d\nLocation: %s,%s\nAltitude: %s", hour, min, sec, sats, lat, lng, alt);
	i2c_write((char *)UPDATE_DATA, params);
	return CMD_OK;
    }
    else {
	LOGE(tag, "Invalid params!");
	return CMD_FAIL;
    }
}

int enable_low_power_mode(char *fmt, char *params, char nparams) {
    LOGI(tag, "Enabling low power mode on FOD");
    if (i2c_write((char *)EMABLE_LOW_POWER_MODE, NULL))
	return CMD_OK;
    return CMD_FAIL;
}

int disable_low_power_mode(char *fmt, char *params, char nparams) {
    LOGI(tag, "Disabling low power mode on FOD");
    if (i2c_write((char *)DISABLE_LOW_POWER_MODE, NULL))
	return CMD_OK;
    return CMD_FAIL;
}

int get_version(char *fmt, char *params, char nparams) {
    LOGI(tag, "Getting FOD's version");
    if (i2c_write((char *)GET_VERSION, NULL)) {
	char ver[4];
	i2c_read(ver);
	LOGI(tag, "FOD's version: %s", &ver)
	return CMD_OK;
    }
    return CMD_FAIL;
}

uint8_t i2c_init(void) {
    if (!bcm2835_init()) {
        printf("bcm2835_init failed. Are you running as root??\n");
        return 0;
    }

    // I2C begin
    if (!bcm2835_i2c_begin()) {
        printf("bcm2835_i2c_begin failed. Are you running as root??\n");
        return 0;
    }
    bcm2835_i2c_setClockDivider(clk_div);
    return 1;
}

int i2c_write(char cmd[], char params[]) {
    uint8_t init_ok = i2c_init();
    printf("init_ok = %d\n", init_ok);
    if (init_ok) {
        bcm2835_i2c_setSlaveAddress(0x07);
	if (params == NULL) {
	    char data[1];
	    sprintf(data, "%d", cmd);
	}
	else {
	    char data[sizeof(cmd) + 1 + sizeof(params)];
	    sprintf(data, "%d %s", cmd, params);
	}
        uint8_t result = bcm2835_i2c_write(data, sizeof(data));
        printf("I2C Write Result = %d\n", result);
        bcm2835_i2c_end();
        bcm2835_close();
        return 1;
    }
    return 0;
}

int i2c_read(char buf[]) {
    uint8_t init_ok = i2c_init();
    printf("init_ok = %d\n", init_ok);
    if (init_ok) {
        for (uint8_t i = 0; i < sizeof(buf); i++) buf[i] = 'n';
        uint8_t data = bcm2835_i2c_read(buf, sizeof(buf));
        printf("I2C Read Result = %d\n", data);
        for (uint8_t i = 0; i < READ_LEN; i++) {
            if (buf[i] != 'n') printf("Read Buf[%d] = %x\n", i, buf[i]);
        }

        // This I2C end is done after a transfer if specified
        bcm2835_i2c_end();
        bcm2835_close();
        return 1;
    }
    return 0;
}
