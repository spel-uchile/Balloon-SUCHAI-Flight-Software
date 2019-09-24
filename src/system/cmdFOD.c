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
char *dev_i2c_1 = (char*)"/dev/i2c-1";
int addr = 0x07;

void cmd_fod_init(void) {
    #ifdef LINUX
    cmd_add("fod_update_data", fod_update_data, "%d %d %d %d %f %f %f", 7);
    cmd_add("fod_send_beacon", fod_send_beacon, "", 0);
    cmd_add("fod_deploy", deploy_femtosats, "", 0);
    cmd_add("fod_get_status", fod_get_status, "", 0);
    cmd_add("fod_get_femtosat_data", get_femtosat_data, "", 0);
    cmd_add("fod_set_on_time", set_on_time, "%d", 1);
    cmd_add("fod_get_config", fod_get_config, "", 0);
    cmd_add("fod_get_version", fod_get_version, "", 0);
    cmd_add("fod_enable_low_power", enable_low_power_mode, "", 0);
    cmd_add("fod_disable_low_power", disable_low_power_mode, "", 0);
    cmd_add("fod_help", fod_help, "", 0);
    #endif

    #ifdef NANOMIND
        //TO DO
    #endif
}

int fod_update_data(char *fmt, char *params, int nparams) {
    if (params == NULL) {
	LOGE(tag, "NULL params!");
	return CMD_FAIL;
    }
    int hour, min, sec, sats;
    float lat, lng, alt;

    if (sscanf(params, fmt, &hour, &min, &sec, &sats, &lat, &lng, &alt) == nparams) {
	LOGI(tag, "Updating FOD's data:\nHH:MM:SS: %d:%d:%d\nSats: %d\nLocation: %f,%f\nAltitude: %f", hour, min, sec, sats, lat, lng, alt);
	fod_i2c_write(UPDATE_DATA, fmt, params);
	return CMD_OK;
    }
    else {
	LOGE(tag, "Invalid params!");
	return CMD_FAIL;
    }
}

int fod_send_beacon(char *fmt, char *params, int nparams) {
    LOGI(tag, "Sending beacon");
    if (fod_i2c_write(SEND_BEACON, fmt, params))
	return CMD_OK;
    return CMD_FAIL;
}

int deploy_femtosats(char *fmt, char *params, int nparams) {
    LOGI(tag, "Deploying femto-satellites");
    return fod_i2c_write(DEPLOY_FEMTOSATS, fmt, NULL);
}

int fod_get_status(char *fmt, char *params, int nparams) {
    LOGI(tag, "Getting FOD's status");
    if (fod_i2c_write(FOD_GET_STATUS, fmt, NULL)) {
	char status[2];
	int released;
	//delay(20);
	fod_i2c_read(status, 2);
	sscanf(status, "%d", &released);
	LOGI(tag, "Released: %d", released);
	return CMD_OK;
    }
    return CMD_FAIL;
}

int get_femtosat_data(char *fmt, char *params, int nparams) {
    LOGI(tag, "Getting femto-satellites' data");
    if (fod_i2c_write(GET_FEMTOSAT_DATA, fmt, NULL))
	return CMD_OK;
    return CMD_FAIL;
}

int set_on_time(char *fmt, char *params, int nparams) {
    if (params == NULL) {
	LOGE(tag, "NULL params!");
	return CMD_FAIL;
    }
    int on_time;
    
    if (sscanf(params, fmt, &on_time) == nparams) {
	LOGI(tag, "Setting on time to %d", on_time);
	fod_i2c_write(SET_ON_TIME, fmt, params);
	return CMD_OK;
    }
    else {
	LOGE(tag, "Invalid params!");
	return CMD_FAIL;
    }
}

int fod_get_config(char *fmt, char *params, int nparams) {
    LOGI(tag, "Getting FOD's configuration");
    fod_i2c_write(GET_CONFIG, fmt, NULL);
    char buf[14];
    float ver;
    int released;
    int on_time;
    int attempts;
    //delay(30);
    fod_i2c_read(buf, 14);
    sscanf(buf, "%f %d %d %d", &ver, &released, &on_time, &attempts);
    LOGI(tag,
         "\nPayload: FOD\nVersion: %f\nReleased: %d\nOn time: %d ms\nAttempts: %d",
	 ver, released, on_time, attempts);
}

int fod_get_version(char *fmt, char *params, int nparams) {
    LOGI(tag, "Getting FOD's version");
    if (fod_i2c_write(GET_VERSION, fmt, NULL)) {
	char buf[5];
	float ver;
	//delay(20);
	fod_i2c_read(buf, 5);
	sscanf(buf, "%f", &ver);
	LOGI(tag, "FOD's version: %f", ver);
	return CMD_OK;
    }
    return CMD_FAIL;
}

int enable_low_power_mode(char *fmt, char *params, int nparams) {
    LOGI(tag, "Enabling low power mode on FOD");
    if (fod_i2c_write(ENABLE_LOW_POWER_MODE, fmt, NULL))
	return CMD_OK;
    return CMD_FAIL;
}

int disable_low_power_mode(char *fmt, char *params, int nparams) {
    LOGI(tag, "Disabling low power mode on FOD");
    if (fod_i2c_write(DISABLE_LOW_POWER_MODE, fmt, NULL))
	return CMD_OK;
    return CMD_FAIL;
}

int fod_help(char *fmt, char *params, int nparams) {
    fod_i2c_write(HELP, fmt, NULL);
    return CMD_OK;
}

int fod_i2c_read(char* buf, uint32_t len) {
    int i2cHandle;
    if ((i2cHandle = open(dev_i2c_1, O_RDWR)) < 0) {
    	printf("Error opening I2C\n");
	return CMD_ERROR;
    }
    else {
    	if (ioctl(i2cHandle, I2C_SLAVE, addr) < 0) {
	    printf("Error at ioctl\n");
	    return CMD_ERROR;
	}
	else {
	    memset(buf, 0, len);
	    uint8_t bytes_read = read(i2cHandle, buf, len);
	    if (bytes_read != len) {
	    	perror("Failed to read from I2C bus");
		return CMD_FAIL;
	    }
	}
	close(*dev_i2c_1);
	return CMD_OK;
    }
}

int fod_i2c_write(int cmd, char *fmt, char params[]) {
    int i2cHandle;
    if ((i2cHandle = open(dev_i2c_1, O_RDWR)) < 0) {
    	printf("Error opening I2C\n");
	return CMD_ERROR;
    }
    else {
    	if (ioctl(i2cHandle, I2C_SLAVE, addr) < 0) {
	    printf("Error at ioctl\n");
	    return CMD_ERROR;
	}
	else {
	    if (params == NULL) {
 	        char data[2];
	    	sprintf(data, "%d", cmd);
	    	int result = write(i2cHandle, data, sizeof(data));
	    }
	    else {
	    	char new_fmt[2 + sizeof(fmt)];
	    	sprintf(new_fmt, "%s %s", "%d", fmt);
	    	char data[sizeof(cmd) + 1 + strlen(params)];
	    	sprintf(data, "%d %s", cmd, params);
	    	int result = write(i2cHandle, data,  strlen(data));
	    }
	}
	close(*dev_i2c_1);
	return CMD_OK;
    }
}
