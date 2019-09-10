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
char *deviceName = (char*)"/dev/i2c-1";

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
    osDelay(100);
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
    osDelay(100);
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
        //printf("write_result: %d", res_w);
        return res_w;
    }
    return 0;
}

int i2c_write(uint8_t addr, uint8_t data1, uint8_t data2, uint8_t data3)
{
    int i2cHandle;
    if ((i2cHandle = open(deviceName, O_RDWR)) < 0)
    {
        printf("error opening I2C\n");
        return 0;
    }
    else
    {
        if (ioctl(i2cHandle, I2C_SLAVE, addr) < 0) {
            printf("Error at ioctl\n");
            return 0;
        } else {
            char wbuf[3];
            wbuf[0] = data1;
            wbuf[1] = data2;
            wbuf[2] = data3;
            int res_tmp = write(i2cHandle, wbuf, sizeof(wbuf));
            //printf("write_fd_res: %d", res_tmp);
        }
        // Close the i2c device bus
        close(*deviceName);
        return 1;
    }
}

int i2c_read(char buf[])
{
    int i2cHandle;
    if ((i2cHandle = open(deviceName, O_RDWR)) < 0)
    {
        printf("error opening I2C\n");
        return 0;
    }
    else
    {
        if (ioctl(i2cHandle, I2C_SLAVE, BIuC_ADDR) < 0) {
            printf("Error at ioctl\n");
            return 0;
        } else {
            uint8_t buf_len = sizeof(buf);
            memset(buf, 0, buf_len);
            uint8_t bytes_r = read(i2cHandle, buf, buf_len);
            if (bytes_r != buf_len)
            {
                perror("Failed to read from the i2c bus");
                return 0;
            }
            /*printf("buf[0] = %d\n", buf[0]);
            printf("buf[1] = %d\n", buf[1]);
            printf("buf[2] = %d\n", buf[2]);*/
        }
        // Close the i2c device bus
        close(*deviceName);
        return 1;
    }
}