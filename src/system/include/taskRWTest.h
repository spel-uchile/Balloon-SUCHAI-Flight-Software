/**
 * @file  taskRWTest.h
 * @author Gustavo Diaz H - gustavo.diaz@ing.uchile.cl
 * @date 2019
 * @copyright GNU GPL v3
 *
 * This task implements a listener, that change the rw speed at periodical times.
 */

#ifndef T_REACTIONWHEELTEST_H
#define T_REACTIONWHEELTEST_H

#include <stdlib.h>
#include <stdint.h>
#include "osDelay.h"
#include "config.h"

#if SCH_COMM_ENABLE
#include "csp/csp.h"
#endif
#ifdef AVR32
#include "asf.h"
    #include "init.h"
#endif
#ifdef NANOMIND
#include "led.h"
    #include "wdt.h"
#endif

#include "globals.h"

#include "osQueue.h"
#include "osDelay.h"

#include "repoCommand.h"

void taskRWTest(void *param);
void rw_change_system_phase(void);

#endif //T_REACTIONWHEELTEST_H