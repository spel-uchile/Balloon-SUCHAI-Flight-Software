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
    cmd_add("rw_set_speed", rw_set_speed, "%d %d", 2);
    #endif

    #ifdef NANOMIND
        //TO DO
    #endif
}

int rw_sample_speed(char *fmt, char *params, int nparams)
{
    LOGI(tag, "Sampling speeds");
    int res_w1 = i2c_write(BIuC_ADDR, SAMPLE_SPEED_CODE_MOTOR1, 0, 0);
    osDelay(100);
    int res_w2 = i2c_write(BIuC_ADDR, SAMPLE_SPEED_CODE_MOTOR2, 0, 0);
    osDelay(100);
    int res_w3 = i2c_write(BIuC_ADDR, SAMPLE_SPEED_CODE_MOTOR3, 0, 0);
    osDelay(100);

    return res_w1 && res_w2 && res_w3;
}

int rw_sample_current(char *fmt, char *params, int nparams)
{
    LOGI(tag, "Sampling current");

    int res_w1 = i2c_write(BIuC_ADDR, SAMPLE_CURRENT_CODE_MOTOR1, 0, 0);
    osDelay(100);
    int res_w2 = i2c_write(BIuC_ADDR, SAMPLE_CURRENT_CODE_MOTOR2, 0, 0);
    osDelay(100);
    int res_w3 = i2c_write(BIuC_ADDR, SAMPLE_CURRENT_CODE_MOTOR3, 0, 0);
    osDelay(100);

    return res_w1 && res_w2 && res_w3;
}
int rw_get_speed(char *fmt, char *params, int nparams)
{
    LOGI(tag, "Getting all speeds");
    char buf1[READ_LEN];
    char buf2[READ_LEN];
    char buf3[READ_LEN];

    int res_w1 = i2c_write(BIuC_ADDR, SAMPLE_SPEED_CODE_MOTOR1, 0, 0);
    osDelay(100);
    int res_w2 = i2c_write(BIuC_ADDR, SAMPLE_SPEED_CODE_MOTOR2, 0, 0);
    osDelay(100);
    int res_w3 = i2c_write(BIuC_ADDR, SAMPLE_SPEED_CODE_MOTOR3, 0, 0);
    osDelay(100);

    int res_r1 = i2c_read(buf1);
    int res_r2 = i2c_read(buf2);
    int res_r3 = i2c_read(buf3);

    uint16_t speed1 = (buf1[0]<<8) | buf1[1];
    uint16_t speed2 = (buf2[0]<<8) | buf2[1];
    uint16_t speed3 = (buf3[0]<<8) | buf3[1];
    LOGI(tag, "Sampled speed1: %d, speed2: %d, speed3: %d", speed1, speed2, speed3);

    return res_w1 && res_w2 && res_w3 && res_r1 && res_r2 && res_r3;
}
int rw_get_current(char *fmt, char *params, int nparams)
{
    LOGI(tag, "Sampling all currents");
    char buf1[READ_LEN];
    char buf2[READ_LEN];
    char buf3[READ_LEN];

    int res_w1 = i2c_write(BIuC_ADDR, SAMPLE_CURRENT_CODE_MOTOR1, 0, 0);
    osDelay(100);
    int res_w2 = i2c_write(BIuC_ADDR, SAMPLE_CURRENT_CODE_MOTOR2, 0, 0);
    osDelay(100);
    int res_w3 = i2c_write(BIuC_ADDR, SAMPLE_CURRENT_CODE_MOTOR3, 0, 0);
    osDelay(100);

    int res_r1 = i2c_read(buf1);
    int res_r2 = i2c_read(buf2);
    int res_r3 = i2c_read(buf3);

    uint16_t current_aux1 = ((buf1[0]&0x07)<<8) | buf1[1];
    float current1 = 3000*(current_aux1-1023)/2048.0; //[mA]
    uint16_t current_aux2 = ((buf2[0]&0x07)<<8) | buf2[1];
    float current2 = 3000*(current_aux2-1023)/2048.0; //[mA]
    uint16_t current_aux3 = ((buf3[0]&0x07)<<8) | buf3[1];
    float current3 = 3000*(current_aux3-1023)/2048.0; //[mA]

    LOGI(tag, "Sampled current1: %f, current2: %f, current3: %f", current1, current2, current3);

    return res_w1 && res_w2 && res_w3 && res_r1 && res_r2 && res_r3;
}
int rw_set_speed(char *fmt, char *params, int nparams)
{
    LOGI(tag, "Speed command");
    int motor_id;
    int speed;
    if(sscanf(params, fmt, &motor_id, &speed) == nparams)
    {
        LOGI(tag, "Setting motor: %d speed: %d", motor_id, speed);
        int res_w;
        if(motor_id == MOTOR1_ID)
        {
            res_w = i2c_write(BIuC_ADDR, SET_SPEED_CODE_MOTOR1, speed & 0xff, speed >> 8);
        }
        else if(motor_id == MOTOR2_ID)
        {
            res_w = i2c_write(BIuC_ADDR, SET_SPEED_CODE_MOTOR2, speed & 0xff, speed >> 8);
        }
        else if(motor_id == MOTOR3_ID)
        {
            res_w = i2c_write(BIuC_ADDR, SET_SPEED_CODE_MOTOR3, speed & 0xff, speed >> 8);
        }
        printf("write_result: %d", res_w);

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
    //size_t buf_len;
    uint8_t buf_len;
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
            buf_len = sizeof(buf);
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