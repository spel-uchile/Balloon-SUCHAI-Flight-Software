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
    uint8_t init_ok = i2c_init();
    printf("init_ok = %d\n", init_ok);
    if (init_ok)
    {
        bcm2835_i2c_setSlaveAddress(BIuC_ADDR);
        char wbuf[3];
        wbuf[0] = SAMPLE_SPEED_CODE;
        wbuf[1] = 0;
        wbuf[2] = 0;

        uint8_t data = bcm2835_i2c_write(wbuf, 3);
        printf("Write Result = %d\n", data);
        bcm2835_i2c_end();
        bcm2835_close();
        return 1;
    }
    return 0;
}

int rw_sample_current(char *fmt, char *params, int nparams)
{
    LOGI(tag, "Sampling current");
    uint8_t init_ok = i2c_init();
    printf("init_ok = %d\n", init_ok);
    if (init_ok)
    {
        bcm2835_i2c_setSlaveAddress(BIuC_ADDR);
        char wbuf[3];
        wbuf[0] = SAMPLE_CURRENT_CODE;
        wbuf[1] = 0;
        wbuf[2] = 0;

        uint8_t data = bcm2835_i2c_write(wbuf, 3);
        printf("Write Result = %d\n", data);
        bcm2835_i2c_end();
        bcm2835_close();
        return 1;
    }
    return 0;
}
int rw_get_speed(char *fmt, char *params, int nparams)
{
    
}
int rw_get_current(char *fmt, char *params, int nparams)
{
    
}
int rw_set_speed(char *fmt, char *params, int nparams)
{
    LOGI(tag, "Speed command");
    uint8_t init_ok = i2c_init();
    printf("init_ok = %d\n", init_ok);
    uint16_t speed;

    if (init_ok)
    {
        if(sscanf(params, fmt, &speed) == nparams)
        {
            //printf("speed = %d\n", speed);
            LOGI(tag, "Setting speed: %d", speed);
            bcm2835_i2c_setSlaveAddress(BIuC_ADDR);
            char wbuf[3];
            wbuf[0] = SET_SPEED_CODE;
            wbuf[1] = speed&0xff;
            wbuf[2] = speed>>8;

            uint8_t data = bcm2835_i2c_write(wbuf, 3);
            printf("Write Result = %d\n", data);
            bcm2835_i2c_end();
            bcm2835_close();
            return 1;
        }
        return 0;
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