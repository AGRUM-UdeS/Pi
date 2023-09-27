#include "pv_management.h"

#include "context.h"

#define PV_MOVE_PERIOD_MS       (60 * 1000)

#define PV_STATUS_TOPIC         ("Status PV")

#define PV_RANGE_DEGREE         (80)
#define DAY_HOUR                (1)
#define SHADES_OFFSET_DEGREE    (10)

static repeating_timer_t pv_move_timer;

static bool PV_calibration_flag = false;
static bool PV_rotation_flag = false;
static bool PV_badweather_flag = false;
static bool PV_power_decreasing_while_moving = false;

static int16_t pv_current_pos = 0;
static int16_t pv_pos_range = 0;
static int16_t pv_init_pos = 0;

void calibrate_PV_position(void)
{
    // TODO

    // Set total range in deg
    pv_pos_range = 60;
}

static bool pv_move_callback(repeating_timer_t *rt)
{
    printf("PV start moving\n");
    PV_rotation_flag = true;
    return PV_rotation_flag;
}

void PV_management(void *pvParameters)
{
    main_context_t *context = (main_context_t*)pvParameters;

    PV_status_t PV_state = PV_INIT;
    PV_status_t last_PV_state = PV_ERROR;

    weather_status_t weather_status = WEATHER_OK;

    uint8_t lm_pin_value[NB_LIMIT_SWITCH] = {0};
    context->motor_drive_enable = true;

    while(1) {
        last_PV_state = PV_state;

        switch (PV_state) {
            case PV_INIT:
                init_motor();

                // If during the day, continue rotation
                if (daytime()) {
                    if (!add_repeating_timer_ms(-(DAY_HOUR*60*60*1000)/PV_RANGE_DEGREE,
                            pv_move_callback, NULL, &pv_move_timer)) {
                        printf("Failed to add pv move timer\n");
                    }
                }

                // Calibration if button pressed
                if (!gpio_get(CALIBRATION_BUTTON)) {
                    PV_state = PV_CALIBRATION;
                } else {
                     PV_state = PV_IDLE;
                }

                break;

            case PV_IDLE:
                // interface_publish(PV_STATUS_TOPIC, PV_IDLE);
                weather_status = forecast_is_bad_weather(&(context->weather_forecast));

                if (morning_pv_calibration()) {   //si entre 5am et 6am
                    clear_pv_calib_flag();
                    PV_state = PV_CALIBRATION;
                } else if (!context->motor_drive_enable) {
                    PV_state = PV_MOTOR_SHUTDOWN;
                } else if (PV_rotation_flag) {
                    PV_rotation_flag = false;
                    PV_state = PV_DAYROTATION;
                } else if (weather_status != WEATHER_OK) {
                    PV_state = PV_BADWEATHER;
                } else if (PV_power_decreasing_while_moving) {
                    PV_state = PV_BACKTRACKING;
                } else {
                    PV_state = PV_IDLE;
                }

                break;

            case PV_CALIBRATION:
                printf("PV calibration starting\n");
                interface_publish(PV_STATUS_TOPIC, PV_CALIBRATION);
                // Go to final position
                rotate_all_pv(180, COUNTERCLOCKWISE); // Will stop beore 180 deg
                vTaskDelay(5000); // Delay to go away from initial limit switches
                // Stop quand limit switch
                do {
                    vTaskDelay(5);
                    limit_switch_touched(lm_pin_value, NB_LIMIT_SWITCH);
                } while(!(lm_pin_value[0] && lm_pin_value[2] &&
                          lm_pin_value[4] && lm_pin_value[6]));

                // Rotate the whole range
                rotate_all_pv(180, CLOCKWISE); // Will stop before 180 deg
                uint32_t start = to_ms_since_boot(get_absolute_time());
                vTaskDelay(10000); // Delay to go away from final limit switches
                do {
                    vTaskDelay(5);
                    limit_switch_touched(lm_pin_value, NB_LIMIT_SWITCH);
                } while(!(lm_pin_value[1] && lm_pin_value[3] &&
                          lm_pin_value[5] && lm_pin_value[7]));

                uint32_t end = to_ms_since_boot(get_absolute_time());

                // Set position and range
                pv_pos_range = ms2angle(end - start) - 20;
                pv_current_pos = pv_init_pos = -pv_pos_range/2 + 10;

                interface_publish("PV range", (float)pv_pos_range);
                rotate_all_pv(10, COUNTERCLOCKWISE);
                while(all_motor_moving()) {
                    vTaskDelay(5);
                }

                // negative timeout means exact delay (rather than delay between callbacks)
                static bool once = true;
                if (once) {
                    once = false;
                    if (!add_repeating_timer_ms(-(12*60*60*1000)/pv_pos_range,
                            pv_move_callback, NULL, &pv_move_timer)) {
                        printf("Failed to add pv move timer\n");
                    }
                }

                PV_state = PV_IDLE;
                interface_publish(PV_STATUS_TOPIC, PV_IDLE);
                break;

            case PV_DAYROTATION:
                interface_publish(PV_STATUS_TOPIC, PV_DAYROTATION);
                // Measure PV output power
                float before_total_power = 0, after_total_power = 0;
                for (size_t i = 0; i < NB_PV; i++) {
                    before_total_power += (get_PV_current(i) * get_PV_voltage(i));
                }

                // rotate PV 1 deg
                rotate_all_pv(1, COUNTERCLOCKWISE);

                // PWM started, wait for motor to stop
                while(all_motor_moving()){
                    vTaskDelay(5);
                    limit_switch_touched(lm_pin_value, NB_LIMIT_SWITCH);
                }
                interface_publish("PV position", (float)(++pv_current_pos));

                //Measure PV output power
                for (size_t i = 0; i < NB_PV; i++) {
                    after_total_power += (get_PV_current(i) * get_PV_voltage(i));
                }

                // backtrack if power reduce
                if (after_total_power < (before_total_power * 0.80)) {
                    PV_state = PV_BACKTRACKING;  
                    break;  
                }

                // If rotation over, go back to 0 deg
                if (pv_current_pos >= pv_pos_range/2 || !(daytime())) {
                    // Turn of timer
                    cancel_repeating_timer(&pv_move_timer);

                    // Make sure you are at the end of rotation
                    rotate_all_pv(180, COUNTERCLOCKWISE);
                    do {
                        vTaskDelay(5);
                        limit_switch_touched(lm_pin_value, NB_LIMIT_SWITCH);
                    } while(!(lm_pin_value[0] && lm_pin_value[2] &&
                            lm_pin_value[4] && lm_pin_value[6]));
                    
                    // Then move back to the middle
                    rotate_all_pv((PV_RANGE_DEGREE/2) + SHADES_OFFSET_DEGREE, CLOCKWISE);
                    while(all_motor_moving()){
                        limit_switch_touched(lm_pin_value, NB_LIMIT_SWITCH);
                    }
                    // Turn of timer
                    cancel_repeating_timer(&pv_move_timer);
                }

                PV_state = PV_IDLE;
                interface_publish(PV_STATUS_TOPIC, PV_IDLE);
                break;

            case PV_BADWEATHER:
                interface_publish(PV_STATUS_TOPIC, PV_BADWEATHER);
                if (weather_status == WEATHER_WIND) {
                    // Go back to initial position
                    rotate_all_pv(pv_current_pos - pv_init_pos, CLOCKWISE);

                    while(all_motor_moving()) {
                        vTaskDelay(100);
                    }

                    pv_current_pos = pv_init_pos;

                    PV_state = PV_IDLE;
                } else if (weather_status == WEATHER_RAIN) {
                    // Straight to catch rainwater
                    if (pv_current_pos > 0) {
                        rotate_all_pv(pv_current_pos, CLOCKWISE); 
                    } else if (pv_current_pos < 0) {
                        rotate_all_pv(-pv_current_pos, COUNTERCLOCKWISE);
                    } else {
                        // Already straight
                        PV_state = PV_IDLE;
                        interface_publish(PV_STATUS_TOPIC, PV_IDLE);
                        break;
                    }

                    while(all_motor_moving()) {
                        vTaskDelay(100);
                    }

                    pv_current_pos = 0;
                    PV_state = PV_IDLE;
                    interface_publish(PV_STATUS_TOPIC, PV_IDLE);
                } else {
                    // Error
                    PV_state = PV_ERROR;
                }
                
                break;

            case PV_BACKTRACKING:
                interface_publish(PV_STATUS_TOPIC, PV_BACKTRACKING);
                
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
                interface_publish(PV_STATUS_TOPIC, PV_IDLE);
                break;

            case PV_MOTOR_SHUTDOWN:
                interface_publish(PV_STATUS_TOPIC, PV_MOTOR_SHUTDOWN);
                rotate_all_pv(150, COUNTERCLOCKWISE);
                while(1) { // Critical error, stay here until reboot
                    vTaskDelay(60000);
                    interface_publish(PV_STATUS_TOPIC, PV_ERROR);
                }
                
                // Continue normal behavior after this
                PV_state = PV_IDLE;
                interface_publish(PV_STATUS_TOPIC, PV_IDLE);
                break;

            case PV_ERROR:
                interface_publish(PV_STATUS_TOPIC, PV_ERROR);
                // Print error message on thingsboard
                PV_state = PV_INIT;

                break;
        }
        last_PV_state = PV_state;
        context->PV_status = PV_state;
        vTaskDelay(1000);
    }
}
