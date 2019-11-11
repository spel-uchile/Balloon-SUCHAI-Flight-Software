/*                                 SUCHAI
 *                      NANOSATELLITE FLIGHT SOFTWARE
 *
 *      Copyright 2018, Carlos Gonzalez Cortes, carlgonz@uchile.cl
 *      Copyright 2018, Tomas Opazo Toro, tomas.opazo.t@gmail.com
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

#include "taskRWTest.h"

static const char *tag = "reactionWheelTest";

enum phase_n{
    phase_a0, // 0: (A0) testing,       [0, 10]      min  -->10 min
    phase_a1, // 1: (A1) speed 1,       [11, 41]     min  -->30 min
    phase_a,  // 2: (A) speed 2,        [42, 1h12m]  min  -->30 min
    phase_b,  // 3: (B) speed 3,        [1h13, 1h43]  min -->30 min
    phase_b1, // 4: (B1) speed 4,       [1h44, 2h14] min  -->30 min
    phase_b2, // 5: (B2) speed 5,       [2h15, 2h45] min  -->30 min
    phase_c,  // 6: (C) speed 6,        [2h46, 3h16] min  -->30 min
    phase_c1, // 7: (C1) speed 7        [3h17, inf]  min  -->
};

/*test phases*/
const int  RW_MIN_PHASE_A  =  1;
const int  RW_MIN_PHASE_A1 =  3;
const int  RW_MIN_PHASE_B  = 6;
const int  RW_MIN_PHASE_B1 = 9;
const int  RW_MIN_PHASE_B2 = 12;
const int  RW_MIN_PHASE_C  = 15;
const int  RW_MIN_PHASE_C1 = 18;

/*const int  RW_MIN_PHASE_A  =  10;
const int  RW_MIN_PHASE_A1 =  41;
const int  RW_MIN_PHASE_B  = 72;
const int  RW_MIN_PHASE_B1 = 103;
const int  RW_MIN_PHASE_B2 = 134;
const int  RW_MIN_PHASE_C  = 165;
const int  RW_MIN_PHASE_C1 = 196;*/

void taskRWTest(void *param)
{
    LOGI(tag, "Started");

    portTick delay_ms    = 1000;            //Task period in [ms]

    unsigned int elapsed_sec = 0;           // Seconds counter
    unsigned int _10sec_check = 10;         //10[s] condition
    unsigned int _01min_check = 1*60;       //1[m] condition
    unsigned int _02min_check = 2*60;       //2[m] condition
    unsigned int _05min_check = 5*60;       //5[m] condition
    unsigned int _10min_check = 10*60;      //10[m] condition
    unsigned int _15min_check = 15*60;      //15[m] condition

    unsigned int _deploy_height = 10210;    //10.21[Km] height deploy condition
    unsigned int _deploy_height_max = 10310;//10.31[Km] error height deploy condition

    int phase = dat_get_system_var(dat_balloon_phase);      // current phase

    portTick xLastWakeTime = osTaskGetTickCount();

    while(1)
    {
        osTaskDelayUntil(&xLastWakeTime, delay_ms); // Suspend task
        elapsed_sec++; //= delay_ms/1000; // Update seconds counts

        LOGD(tag, "elapsed second %u", elapsed_sec);
        rw_change_system_phase();
        phase = dat_get_system_var(dat_balloon_phase); // Determine current phase

        /**
         * Phases Setup
         */

        /**
         * In Phase A0: Test commands, 10 min
            -get_speed
            -get_current
            -set_speed
        */
        /*if(phase == phase_a0) {
            if ((elapsed_sec % _01min_check) == 0) {
                cmd_t *cmd_set_speed = cmd_get_str("rw_set_speed");
                cmd_add_params_str(cmd_set_speed, "1 100");
                cmd_send(cmd_set_speed);
            }
        }

        *//**
         * In Phase A1: Set speed at
        *//*
        if(phase == phase_a1) {
            if ((elapsed_sec % _01min_check) == 0) {
                cmd_t *cmd_set_speed = cmd_get_str("rw_set_speed");
                cmd_add_params_str(cmd_set_speed, "1 158");
                cmd_send(cmd_set_speed);
            }
        }

        *//**
         * In Phase A:
        *//*
        if(phase == phase_a) {
            if ((elapsed_sec % _01min_check) == 0) {
                cmd_t *cmd_set_speed = cmd_get_str("rw_set_speed");
                cmd_add_params_str(cmd_set_speed, "1 217");
                cmd_send(cmd_set_speed);
            }
        }

        *//**
         * In Phase B:
        *//*
        if(phase == phase_b) {
            if ((elapsed_sec % _01min_check) == 0) {
                cmd_t *cmd_set_speed = cmd_get_str("rw_set_speed");
                cmd_add_params_str(cmd_set_speed, "1 276");
                cmd_send(cmd_set_speed);
            }
        }

        *//**
         * In Phase B1:
        *//*
        if(phase == phase_b1) {
            if ((elapsed_sec % _01min_check) == 0) {
                cmd_t *cmd_set_speed = cmd_get_str("rw_set_speed");
                cmd_add_params_str(cmd_set_speed, "1 334");
                cmd_send(cmd_set_speed);
            }
        }

        *//**
         * In Phase B2:
        *//*
        if(phase == phase_b2) {
            if ((elapsed_sec % _01min_check) == 0) {
                cmd_t *cmd_set_speed = cmd_get_str("rw_set_speed");
                cmd_add_params_str(cmd_set_speed, "1 393");
                cmd_send(cmd_set_speed);
            }
        }

        *//**
         * In Phase C:
        *//*
        if(phase == phase_c) {
            if ((elapsed_sec % _01min_check) == 0) {
                cmd_t *cmd_set_speed = cmd_get_str("rw_set_speed");
                cmd_add_params_str(cmd_set_speed, "1 452");
                cmd_send(cmd_set_speed);
            }
        }

        *//**
         * In Phase C1:
        *//*
        if(phase == phase_c1) {
            if ((elapsed_sec % _01min_check) == 0) {
                cmd_t *cmd_set_speed = cmd_get_str("rw_set_speed");
                cmd_add_params_str(cmd_set_speed, "1 511");
                cmd_send(cmd_set_speed);
            }
        }*/

        /**
         * Always to do list
         * -get_speed
         * -get_current
        */
        /*if ((elapsed_sec % 5) == 2) {
            cmd_t *cmd_get_speed = cmd_get_str("rw_get_speed");
            cmd_send(cmd_get_speed);
        }

        if ((elapsed_sec % 5) == 3) {
            cmd_t *cmd_get_current = cmd_get_str("rw_get_current");
            cmd_send(cmd_get_current);
        }*/

        if ((elapsed_sec % 1) == 0) {
            cmd_t *cmd_set_speed = cmd_get_str("rw_set_speed");
            cmd_add_params_str(cmd_set_speed, "1 158");
            cmd_send(cmd_set_speed);
        }

        /* 1 min actions, update minutes alive counter*/
        if((elapsed_sec % _01min_check) == 0)
        {
            LOGD(tag, "1 hour check");
            cmd_t *cmd_1h = cmd_get_str("drp_add_hrs_alive");
            cmd_add_params_var(cmd_1h, 1); // Add 1 min
            cmd_send(cmd_1h);
            LOGI(tag, "Phase: %d", phase);
        }
    }
}

void rw_change_system_phase()
{
    int current_phase = dat_get_system_var(dat_balloon_phase);
    int min_alive= dat_get_system_var(dat_obc_hrs_alive);
    LOGD(tag, "minutes alive:  %d", min_alive);

    if( min_alive == -1)
        return;

    if(min_alive <= RW_MIN_PHASE_A)
    {
        LOGD(tag, "######################### We are in PHASE A0 #########################");
        // We are in phase A0
        if(current_phase != phase_a0) {
            dat_set_system_var(dat_balloon_phase, phase_a0);
        }
    }
    else if (min_alive > RW_MIN_PHASE_A && min_alive <= RW_MIN_PHASE_A1)
    {
        LOGD(tag, "######################### We are in PHASE A1 #########################");
        // We are in phase A1
        if(current_phase != phase_a1){
            dat_set_system_var(dat_balloon_phase, phase_a1);

        }
    }
    else if (min_alive > RW_MIN_PHASE_A1 && min_alive <= RW_MIN_PHASE_B)
    {
        LOGD(tag, "######################### We are in PHASE A #########################");
        // We are in phase A
        if(current_phase != phase_a){
            dat_set_system_var(dat_balloon_phase, phase_a);

        }
    }
    else if (min_alive > RW_MIN_PHASE_B && min_alive <= RW_MIN_PHASE_B1)
    {
        LOGD(tag, "######################### We are in PHASE B #########################");
        // We are in phase B
        if(current_phase != phase_b){
            dat_set_system_var(dat_balloon_phase, phase_b);
        }

    }
    else if (min_alive > RW_MIN_PHASE_B1 && min_alive <= RW_MIN_PHASE_B2)
    {
        LOGD(tag, "######################### We are in PHASE B1 #########################");
        // We are in phase B1
        if(current_phase != phase_b1){
            dat_set_system_var(dat_balloon_phase, phase_b1);
        }

    }
    else if (min_alive > RW_MIN_PHASE_B2 && min_alive <= RW_MIN_PHASE_C)
    {
        LOGD(tag, "######################### We are in PHASE B2 #########################");
        // We are in phase B2
        if(current_phase != phase_b2){
            dat_set_system_var(dat_balloon_phase, phase_b2);
        }

    }
    else if (min_alive > RW_MIN_PHASE_C && min_alive <= RW_MIN_PHASE_C1)
    {
        LOGD(tag, "######################### We are in PHASE C #########################");
        // We are in phase C
        if(current_phase != phase_c) {
            dat_set_system_var(dat_balloon_phase, phase_c);
        }

    }
    else if (min_alive > RW_MIN_PHASE_C1)
    {
        LOGD(tag, "######################### We are in PHASE C1 #########################");
        // We are in phase C1
        if(current_phase != phase_c1){
            dat_set_system_var(dat_balloon_phase, phase_c1);
        }
    }
}
