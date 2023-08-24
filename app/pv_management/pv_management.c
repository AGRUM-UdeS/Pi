#include "pv_management.h"

#include "context.h"

static bool PV_calibration_flag = false;
static bool PV_rotation_flag = false;
static bool PV_badweather_flag = false;
static bool PV_power_decreasing_while_moving = false;

void calibrate_PV_position(void)
{
    // TODO
}

static void pv_calibration_alarm_cb(void)
{
    PV_calibration_flag = true;
}

void PV_management(void *pvParameters)
{
    main_context_t *context = (main_context_t*)pvParameters;

    PV_status_t PV_state = PV_INIT;
    PV_status_t last_PV_state = PV_ERROR;

    init_motor();
    calibrate_PV_position();

    // Alarm once a day
    datetime_t pv_calibration_alarm = {
        .year  = -1,
        .month = -1,
        .day   = -1,
        .dotw  = -1,
        .hour  = 5,
        .min   = 30,
        .sec   = 0,
    };

    rtc_set_alarm(&pv_calibration_alarm, &pv_calibration_alarm_cb);

    while(1) {
        last_PV_state = PV_state;

        switch (PV_state) {
            case PV_INIT:
                //JC : Init mosfet states of pumps and valves

                 PV_state = PV_CALIBRATION;
                break;

            case PV_IDLE:
                
                if (PV_calibration_flag) {   //si entre 5am et 6am
                    PV_state = PV_CALIBRATION;
                }
                else if (PV_rotation_flag) {
                    PV_state = PV_DAYROTATION;
                } 
                else if (PV_badweather_flag) {
                    PV_state = PV_BADWEATHER;
                }
                else if (PV_power_decreasing_while_moving) {
                    PV_state = PV_BACKTRACKING;
                }
                else {
                    PV_state = PV_IDLE;
                }


                break;

            case PV_CALIBRATION:
                
                //JC : routine de calibration

                PV_state = PV_IDLE;
                break;

            case PV_DAYROTATION:
                // PV_before = PV_measure(); // measure pv power output
                // motor = one_degree; //on fait tourner les motors de 1deg est à ouest
                // PV_after = PV_measure();
                // if(PV_after < 0.8*PV_before){ //drop de 20%
                //     PV_power_decreasing_while_moving = 1;
                // }
                // else {
                //     PV_power_decreasing_while_moving = 0;
                // }


                PV_state = PV_IDLE;
                break;

            case PV_BADWEATHER:
                
                //Touch limit switch and stay there
                //Only when wind is fucking crazy in the head

                PV_state = PV_IDLE;
                break;

            case PV_BACKTRACKING:
                
                //backtrack, only at the end of the day

                // PV_before = PV_measure();
                // motor = -one_degree; //on fait tourner les motors de 1deg ouest à est
                // PV_after = PV_measure();
                // if(PV_after > PV_before){ //plus de puissance apres avoir tourner qu'avant, on continue à backtrack parce qu'on a pas atteint le max
                //     PV_power_decreasing_while_moving = 1;
                // }
                // else {
                //     PV_power_decreasing_while_moving = 0;
                // }

                PV_state = PV_IDLE;
                break;

            case PV_ERROR:
                // Print error message on thingsboard
                PV_state = PV_INIT;

                break;
        }
        last_PV_state = PV_state;
        context->PV_status = PV_state;
        vTaskDelay(100);
    }
}
