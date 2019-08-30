/**
 * @file  cmdRW.h
 * @author Gustavo Díaz H - g.hernan.diaz@ing.uchile.cl
 * @date 2019
 * @copyright GNU GPL v3
 *
 * This header have definitions of commands related to reaction wheel driver
 * (RW) features.
 */

#ifndef CMD_RW_H
#define CMD_RW_H

#include "config.h"

#ifdef LINUX
    #include <bcm2835.h>
    #include <stdint.h>
#endif


#define MAX_LEN 32
#define READ_LEN 3
#define OBC_ADDR 0x10
#define BIuC_ADDR 0x11
#define SAMPLE_SPEED_CODE 21
#define SAMPLE_CURRENT_CODE 22
#define SET_SPEED_CODE 23

#define DATA_PACKET_SZ 8
typedef struct{
    float speed;
    float current;
}motor_data_t;

motor_data_t motor_data_;

#define CMD_PACKET_SZ 2
typedef struct{
    uint16_t data;
}data_t;

data_t data_;

#include "repoCommand.h"
#include "os/os.h"

/**
 * Register reaction whee related (rw) commands
 */
void cmd_rw_init(void);

int rw_sample_speed(char *fmt, char *params, int nparams);

/**
 * Request speed to the driver and store in their internal registers
 *
 * @param fmt Str. Parameters format ""
 * @param params Str. Parameters as string ""
 * @param nparams Int. Number of parameters 0
 * @return  CMD_OK if executed correctly or CMD_FAIL in case of errors
 */
uint8_t i2c_init(void);

/**
 * Init i2c library for raspberry
 */

int rw_sample_current(char *fmt, char *params, int nparams);
int rw_get_speed(char *fmt, char *params, int nparams);
int rw_get_current(char *fmt, char *params, int nparams);
int rw_set_speed(char *fmt, char *params, int nparams);
int i2c_write(uint8_t addr, uint8_t data1, uint8_t data2, uint8_t data3);
int i2c_read(char* buf);

#endif /* CMD_RW_H */