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
#include "repoData.h"

#ifdef LINUX
    #include <stdint.h>
    #include <fcntl.h>
    #include <sys/ioctl.h>
    #include <linux/i2c-dev.h>
    #include <errno.h>
    #include <unistd.h>
#endif

#define UPDATE_DATA 0
#define SEND_BEACON 1
#define DEPLOY_FEMTOSATS 2
#define FOD_GET_STATUS 3
#define GET_FEMTOSAT_DATA 4
#define SET_ON_TIME 5
#define GET_CONFIG 6
#define GET_VERSION 7
#define ENABLE_LOW_POWER_MODE 8
#define DISABLE_LOW_POWER_MODE 9
#define HELP 10

#include "repoCommand.h"
#include "os/os.h"

/**
 * Register femto-satellite orbital deployer commands.
 */
void cmd_fod_init(void);
int fod_update_data(char *fmt, char *params, int nparams);
int fod_send_beacon(char *fmt, char *params, int nparams);
int deploy_femtosats(char *fmt, char *params, int nparams);
int fod_get_status(char *fmt, char *params, int nparams);
int get_femtosat_data(char *fmt, char *params, int nparams); 
int set_on_time(char *fmt, char *params, int nparams);
int fod_get_config(char *fmt, char *params, int nparams);
int fod_get_version(char *fmt, char *params, int nparams);
int enable_low_power_mode(char *fmt, char *params, int nparams);
int disable_low_power_mode(char *fmt, char *params, int nparams);
int fod_help(char *fmt, char *params, int nparams);

//int rw_sample_speed(char *fmt, char *params, int nparams);

/**
 * Request speed to the driver and store in their internal registers
 *
 * @param fmt Str. Parameters format ""
 * @param params Str. Parameters as string ""
 * @param nparams Int. Number of parameters 0
 * @return  CMD_OK if executed correctly or CMD_FAIL in case of errors
 */
uint8_t fod_i2c_init(void);

/**
 * Init i2c library for raspberry
 */

int fod_i2c_write(int cmd, char *fmt, char params[]);
int fod_i2c_read(char* buf, uint32_t len);

#endif /* CMD_FOD_H */
