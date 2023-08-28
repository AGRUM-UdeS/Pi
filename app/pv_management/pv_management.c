#include "pv_management.h"

#include "context.h"

static bool PV_calibration_flag = false;
static bool PV_rotation_flag = false;
static bool PV_badweather_flag = false;
static bool PV_power_decreasing_while_moving = false;

static int16_t pv_current_pos = 0;
static int16_t pv_pos_range = 0;
static int16_t pv_init_pos = -30;

void calibrate_PV_position(void)
{
    // TODO

    // Set total range in deg
    pv_pos_range = 60;
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

    weather_status_t weather_status = WEATHER_OK;

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
                init_motor();

                PV_state = PV_CALIBRATION;
                break;

            case PV_IDLE:
                weather_status = forecast_is_bad_weather(&(context->weather_forecast));

                if (PV_calibration_flag) {   //si entre 5am et 6am
                    PV_state = PV_CALIBRATION;
                }
                else if (PV_rotation_flag) {
                    PV_rotation_flag = false;
                    PV_state = PV_DAYROTATION;
                } 
                else if (weather_status != WEATHER_OK) {
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
                // Aller a droite au fond

                // Stop quand limit switch

                // Set position initiale
                pv_current_pos = -30;

                PV_state = PV_IDLE;
                break;

            case PV_DAYROTATION:
                // Measure PV output power
                float before_total_power = 0, after_total_power = 0;
                for (size_t i = 0; i < NB_PV; i++) {
                    before_total_power += (get_PV_current(i) * get_PV_voltage(i));
                }

                // rotate PV 1 deg
                rotate_all_pv(1, CLOCKWISE);

                // PWM started, wait for motor to stop
                while(all_motor_moving()){
                    vTaskDelay(100);
                }

                //Measure PV output power
                for (size_t i = 0; i < NB_PV; i++) {
                    after_total_power += (get_PV_current(i) * get_PV_voltage(i));
                }

                // backtrack if power reduce
                if (after_total_power < (before_total_power * 0.80)) {
                    PV_state = PV_BACKTRACKING;    
                }

                PV_state = PV_IDLE;
                break;

            case PV_BADWEATHER:
                if (weather_status == WEATHER_WIND) {
                    // Go back to initial position
                    rotate_all_pv(pv_current_pos - pv_init_pos, COUNTERCLOCKWISE);

                    while(all_motor_moving()) {
                        vTaskDelay(100);
                    }

                    pv_current_pos = pv_init_pos;

                    PV_state = PV_IDLE;
                } else if (weather_status == WEATHER_RAIN) {
                    // Straight to catch rainwater
                    if (pv_current_pos > 0) {
                        rotate_all_pv(pv_current_pos, COUNTERCLOCKWISE); 
                    } else if (pv_current_pos < 0) {
                        rotate_all_pv(-pv_current_pos, CLOCKWISE);
                    } else {
                        // Already straight
                        PV_state = PV_IDLE;
                        break;
                    }

                    while(all_motor_moving()) {
                        vTaskDelay(100);
                    }

                    pv_current_pos = 0;
                    PV_state = PV_IDLE;
                } else {
                    // Error
                    PV_state = PV_ERROR;
                }
                
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
        vTaskDelay(1000);
    }
}
