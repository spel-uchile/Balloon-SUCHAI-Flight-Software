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

#include "cmdRW.h"

static const char* tag = "cmdRW";
uint16_t clk_div = BCM2835_I2C_CLOCK_DIVIDER_148;

void cmd_rw_init(void)
{
    #ifdef LINUX
    cmd_add("rw_sample_speed", rw_sample_speed, "", 0);
    cmd_add("rw_sample_current", rw_sample_current, "", 0);
    cmd_add("rw_get_speed", rw_get_speed, "", 0);
    cmd_add("rw_get_current", rw_get_current, "", 0);
    cmd_add("rw_set_speed", rw_set_speed, "%d", 1);
    #endif

    #ifdef NANOMIND
        //TO DO
    #endif
}

int rw_sample_speed(char *fmt, char *params, int nparams)
{
    LOGI(tag, "Sampling speed");
    int res = i2c_write(BIuC_ADDR, SAMPLE_SPEED_CODE, 0, 0);
    return res;
}

int rw_sample_current(char *fmt, char *params, int nparams)
{
    LOGI(tag, "Sampling current");
    int res = i2c_write(BIuC_ADDR, SAMPLE_CURRENT_CODE, 0, 0);
    return res;
}
int rw_get_speed(char *fmt, char *params, int nparams)
{
    LOGI(tag, "Getting speed");
    char buf[READ_LEN];
    int res_w = i2c_write(BIuC_ADDR, SAMPLE_SPEED_CODE, 0, 0);
    delay(20);
    int res_r = i2c_read(buf);
    uint16_t speed = (buf[0]<<8) | buf[1];
    LOGI(tag, "Sampled speed: %d", speed);
    return res_w && res_r;
}
int rw_get_current(char *fmt, char *params, int nparams)
{
    LOGI(tag, "Sampling speed");
    char buf[READ_LEN];
    int res_w = i2c_write(BIuC_ADDR, SAMPLE_CURRENT_CODE, 0, 0);
    delay(20);
    int res_r = i2c_read(buf);
    uint16_t current_aux = ((buf[0]&0x07)<<8) | buf[1];
    LOGI(tag, "current aux: %d", current_aux);
    float current = 3000*(current_aux-1023)/2048.0; //[mA]
    LOGI(tag, "Sampled current: %f", current);
    return res_w && res_r;
}
int rw_set_speed(char *fmt, char *params, int nparams)
{
    LOGI(tag, "Speed command");
    uint16_t speed;
    if(sscanf(params, fmt, &speed) == nparams)
    {
        LOGI(tag, "Setting speed: %d", speed);
        int res_w = i2c_write(BIuC_ADDR, SET_SPEED_CODE, speed&0xff, speed>>8);
        return res_w;
    }
    return 0;
}

uint8_t i2c_init(void)
{
    if (!bcm2835_init())
    {
        printf("bcm2835_init failed. Are you running as root??\n");
        return 0;
    }

    // I2C begin
    if (!bcm2835_i2c_begin())
    {
        printf("bcm2835_i2c_begin failed. Are you running as root??\n");
        return 0;
    }
    bcm2835_i2c_setClockDivider(clk_div);
    return 1;
}

int i2c_write(uint8_t addr, uint8_t data1, uint8_t data2, uint8_t data3)
{
    uint8_t init_ok = i2c_init();
    printf("init_ok = %d\n", init_ok);
    if (init_ok)
    {
        bcm2835_i2c_setSlaveAddress(addr);
        char wbuf[3];
        wbuf[0] = data1;
        wbuf[1] = data2;
        wbuf[2] = data3;

        uint8_t data = bcm2835_i2c_write(wbuf, 3);
        printf("I2C Write Result = %d\n", data);
        bcm2835_i2c_end();
        bcm2835_close();
        return 1;
    }
    return 0;
}

int i2c_read(char buf[])
{
    uint8_t init_ok = i2c_init();
    printf("init_ok = %d\n", init_ok);
    if (init_ok)
    {
        for (uint8_t i = 0; i < READ_LEN; i++) buf[i] = 'n';
        uint8_t data = bcm2835_i2c_read(buf, 2);
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