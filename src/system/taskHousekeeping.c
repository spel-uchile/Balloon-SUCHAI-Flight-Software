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

#include "taskHousekeeping.h"

static const char *tag = "Housekeeping";

/* Flight Plan*/

enum phase_n{
    phase_a0, // 0: (A0) testing,       [0, 20]     min  -->20 min
    phase_a1, // 1: (A1) preparation,   [21, 26]    min  -->05 min
    phase_a,  // 2: (A) ascend,         [27, 60]    min  -->33 min
    phase_b,  // 3: (B) equilibrium,    [61, 95]    min  -->34 min
    phase_b1, // 4: (B1) deploy1,       [96, 101]   min  -->05 min
    phase_b2, // 5: (B2) deploy2,       [102, 107]  min  -->05 min
    phase_c,  // 6: (C) descend,        [108, 138]  min  -->30 min
    phase_c1, // 7: (C1) landing        [139, inf]  min  -->
};


const int  MIN_PHASE_A  =  20;
const int  MIN_PHASE_A1 =  26;
const int  MIN_PHASE_B  = 60;
const int  MIN_PHASE_B1 = 95;
const int  MIN_PHASE_B2 = 101;
const int  MIN_PHASE_C  = 107;
const int  MIN_PHASE_C1 = 138;

void taskHousekeeping(void *param)
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

    prs_data_t prs_data_;
    
    while(1)
    {
        osTaskDelayUntil(&xLastWakeTime, delay_ms); // Suspend task
        elapsed_sec++; //= delay_ms/1000; // Update seconds counts

        /**
         * Phases Setup
         */
        if (elapsed_sec == 1 || elapsed_sec == 2 || elapsed_sec == 3 || elapsed_sec == 4 ) {
            cmd_t *cmd_open_la = cmd_get_str("open_dpl_la");
            cmd_send(cmd_open_la);

            /*set pi-camera looking at the horizon*/
            cmd_t *cmd_close_sm = cmd_get_str("close_dpl_sm");
            cmd_send(cmd_close_sm);
        }

        /**
         * In all Phases sample prs and gps every 10 seconds
         * sample dpl every 1 min
         */
        LOGD(tag, "elapsed second %u", elapsed_sec);
        change_system_phase();
        phase = dat_get_system_var(dat_balloon_phase); // Determine current phase

        if ((elapsed_sec % _10sec_check) == 1) {
            cmd_t *cmd_get_gps = cmd_get_str("get_gps_data");
            cmd_send(cmd_get_gps);
        }

        if ((elapsed_sec % _10sec_check) == 4) {
            cmd_t *cmd_get_prs = cmd_get_str("get_prs_data");
            cmd_send(cmd_get_prs);
        }

        if ((elapsed_sec % _01min_check) == 0) {
            cmd_t *cmd_get_dpl = cmd_get_str("get_dpl_data");
            cmd_send(cmd_get_dpl);
        }


        /**
         * In Phase A0: Test main systems, 20 min
            -deploy actuator
            -iridium module
            -camera
            -sstv
        */
        if(phase == phase_a0) {
            /*Test open Lineal Actuator*/
            if ((elapsed_sec % _01min_check) == 0) {
                cmd_t *cmd_open_dpl_1 = cmd_get_str("open_dpl_1");
                cmd_send(cmd_open_dpl_1);
            }

            if ((elapsed_sec % _01min_check) == 0) {
                cmd_t *cmd_open_sm = cmd_get_str("open_dpl_sm");
                cmd_send(cmd_open_sm);
            }

            if ((elapsed_sec % _01min_check) == 30) {
                cmd_t *cmd_close_sm = cmd_get_str("close_dpl_sm");
                cmd_send(cmd_close_sm);
            }

            if ((elapsed_sec % _05min_check) == 0) {
                cmd_t *cmd_run_sstv = cmd_get_str("run_sstv");
                cmd_send(cmd_run_sstv);
            }
        }

        /**
         * In Phase A1: Set dafualts values, 05 min
            -deploy system
            -camera point at horizon
        */
        if(phase == phase_a1) {
            /*Test open Lineal Actuator*/
            if ((elapsed_sec % _01min_check) == 0) {
                cmd_t *cmd_open_dpl_1 = cmd_get_str("open_dpl_1");
                cmd_send(cmd_open_dpl_1);
            }

            if ((elapsed_sec % _01min_check) == 0) {
                cmd_t *cmd_close_sm = cmd_get_str("close_dpl_sm");
                cmd_send(cmd_close_sm);
            }

            if ((elapsed_sec % _01min_check) == 0) {
                cmd_t *cmd_send_iridium_msg1 = cmd_get_str("send_iridium_msg1");
                cmd_send(cmd_send_iridium_msg1);
            }
        }

        /**
         * In Phase A: ascend, 88 min
         * sstv
        */
        if(phase == phase_a) {
            if ((elapsed_sec % _15min_check) == 0) {
                cmd_t *cmd_run_sstv = cmd_get_str("run_sstv");
                cmd_send(cmd_run_sstv);
            }
        }

        /**
         * In Phase B: equilibrium, 58 min
         * sstv
        */
        if(phase == phase_b) {
            if ((elapsed_sec % _15min_check) == 0) {
                cmd_t *cmd_run_sstv = cmd_get_str("run_sstv");
                cmd_send(cmd_run_sstv);
            }
        }

        /**
             * In Phase B1: deploy, 05 min
                -point pi-camera to the starts (try cmd every 1 min)
                -deploy linear actuator every 10 seconds
                -send data through iridium
            */
        if(phase == phase_b1 || (prs_data_.height>=_deploy_height && prs_data_.height<_deploy_height_max)) {
            if ((elapsed_sec % _01min_check) == 0) {
                cmd_t *cmd_open_sm = cmd_get_str("open_dpl_sm");
                cmd_send(cmd_open_sm);
            }
            if(phase == phase_b1) {
                if ((elapsed_sec % 10) == 0) {
                    cmd_t *cmd_open_dpl_1 = cmd_get_str("open_dpl_1");
                    cmd_send(cmd_open_dpl_1);
                }
                if ((elapsed_sec % _02min_check) == 0) {
                    cmd_t *cmd_run_sstv = cmd_get_str("run_sstv");
                    cmd_send(cmd_run_sstv);
                }
            }
        }

        /**
         * In Phase B2: deploy, 05 min
            -point pi-camera to the starts (try cmd every 1 min)
        */
        if(phase == phase_b2) {
            if ((elapsed_sec % _01min_check) == 0) {
                cmd_t *cmd_open_sm = cmd_get_str("open_dpl_sm");
                cmd_send(cmd_open_sm);
            }
            if ((elapsed_sec % _02min_check) == 0) {
                cmd_t *cmd_run_sstv = cmd_get_str("run_sstv");
                cmd_send(cmd_run_sstv);
            }
        }

        /**
             * In Phase C: descend, 30 min
                -point pi-camera to the horizon (try cmd every 10 seconds)
                -send img (sstv)
            */
        if(phase == phase_c) {
            /*looking to the horizon*/
            if ((elapsed_sec % _05min_check) == 0) {
                cmd_t *cmd_close_sm = cmd_get_str("close_dpl_sm");
                cmd_send(cmd_close_sm);
            }
            if ((elapsed_sec % _05min_check) == 150) {
                cmd_t *cmd_open_sm = cmd_get_str("open_dpl_sm");
                cmd_send(cmd_open_sm);
            }
            if ((elapsed_sec % _10min_check) == 0) {
                cmd_t *cmd_run_sstv = cmd_get_str("run_sstv");
                cmd_send(cmd_run_sstv);
            }
        }

        /**
         * In Phase C1: landing,
            -send img (sstv)
        */
        if(phase == phase_c1) {
            if ((elapsed_sec % _10min_check) == 0) {
                cmd_t *cmd_run_sstv = cmd_get_str("run_sstv");
                cmd_send(cmd_run_sstv);
            }
        }

        /**
         * Always to do list
        */
        if((elapsed_sec % _10min_check) == 0)
        {
            dat_get_recent_payload_sample(&prs_data_, prs_sensors, 1);
        }
        /* 1 min actions, update minutes alive counter*/
        if((elapsed_sec % _01min_check) == 0)
        {
            LOGD(tag, "1 hour check");
            cmd_t *cmd_1h = cmd_get_str("update_hours_alive");
            cmd_add_params_var(cmd_1h, 1); // Add 1 min
            cmd_send(cmd_1h);
            LOGI(tag, "Phase: %d", phase);
        }
    }
}

void change_system_phase()
{
    int current_phase = dat_get_system_var(dat_balloon_phase);
    int min_alive= dat_get_system_var(dat_obc_hrs_alive);
    LOGD(tag, "minutes alive:  %d", min_alive);

    if( min_alive == -1)
        return;

    if(min_alive <= MIN_PHASE_A)
    {
        LOGD(tag, "######################### We are in PHASE A0 #########################");
        // We are in phase A0
        if(current_phase != phase_a0) {
            dat_set_system_var(dat_balloon_phase, phase_a0);
        }
    }
    else if (min_alive > MIN_PHASE_A && min_alive <= MIN_PHASE_A1)
    {
        LOGD(tag, "######################### We are in PHASE A1 #########################");
        // We are in phase A1
        if(current_phase != phase_a1){
            dat_set_system_var(dat_balloon_phase, phase_a1);

        }
    }
    else if (min_alive > MIN_PHASE_A1 && min_alive <= MIN_PHASE_B)
    {
        LOGD(tag, "######################### We are in PHASE A #########################");
        // We are in phase A
        if(current_phase != phase_a){
            dat_set_system_var(dat_balloon_phase, phase_a);

        }
    }
    else if (min_alive > MIN_PHASE_B && min_alive <= MIN_PHASE_B1)
    {
        LOGD(tag, "######################### We are in PHASE B #########################");
        // We are in phase B
        if(current_phase != phase_b){
            dat_set_system_var(dat_balloon_phase, phase_b);
        }

    }
    else if (min_alive > MIN_PHASE_B1 && min_alive <= MIN_PHASE_B2)
    {
        LOGD(tag, "######################### We are in PHASE B1 #########################");
        // We are in phase B1
        if(current_phase != phase_b1){
            dat_set_system_var(dat_balloon_phase, phase_b1);
        }

    }
    else if (min_alive > MIN_PHASE_B2 && min_alive <= MIN_PHASE_C)
    {
        LOGD(tag, "######################### We are in PHASE B2 #########################");
        // We are in phase B2
        if(current_phase != phase_b2){
            dat_set_system_var(dat_balloon_phase, phase_b2);
        }

    }
    else if (min_alive > MIN_PHASE_C && min_alive <= MIN_PHASE_C1)
    {
        LOGD(tag, "######################### We are in PHASE C #########################");
        // We are in phase C
        if(current_phase != phase_c) {
            dat_set_system_var(dat_balloon_phase, phase_c);
        }

    }
    else if (min_alive > MIN_PHASE_C1)
    {
        LOGD(tag, "######################### We are in PHASE C1 #########################");
        // We are in phase C1
        if(current_phase != phase_c1){
            dat_set_system_var(dat_balloon_phase, phase_c1);
        }
    }
}
