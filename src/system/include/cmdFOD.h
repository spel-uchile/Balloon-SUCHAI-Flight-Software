/**
 * @file  cmdFOD.h
 * @author Matías Vidal V - matias.vidal.v@gmail.cl
 * @date 2019
 * @copyright GNU GPL v3
 *
 * This header has definitions of commands related to the Femto-satellite Orbital Deployer driver
 * (FOD) features.
 */

#ifndef CMD_FOD_H
#define CMD_FOD_H

#include "config.h"

#ifdef LINUX
    #include <stdint.h>
#endif

#define HELP 0
#define DEPLOY_FEMTOSATS 1
#define FOD_GET_STATUS 2
#define GET_FEMTOSAT_DATA 3
#define SET_ON_TIME 4
#define SEND_BEACON 5
#define UPDATE_DATA 6
#define ENABLE_LOW_POWER_MODE 7
#define DISABLE_LOW_POWER_MODE 8
#define GET_VERSION 9

#include "repoCommand.h"
#include "os/os.h"

/**
 * Register femto-satellite orbital deployer commands.
 */
void cmd_fod_init(void);
int help(char *fmt, char *params, int nparams);
int deploy_femtosats(char *fmt, char *params, char nparams);
int get_status(char *fmt, char *params, char nparams);
int get_femtosat_data(char *fmt, char *params, char nparams); 
int set_on_time(char *fmt, char *params, char nparams);
int send_beacon(char *fmt, char *params, char nparams);
int update_data(char *fmt, char *params, char nparams);
int enable_low_power_mode(char *fmt, char *params, char nparams);
int disable_low_power_mode(char *fmt, char *params, char nparams);
int get_version(char *fmt, char *params, char nparams);


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

int i2c_write(char data[]);
int i2c_read(char* buf)

#endif /* CMD_FOD_H */
