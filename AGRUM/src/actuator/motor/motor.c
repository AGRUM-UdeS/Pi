#include "motor.h"

#define pul_1_pin 12
#define pul_2_pin 13
#define pul_3_pin 14
#define pul_4_pin 15

#define MAX_DUTY_CYCLE 1000
#define DUTY_CYCLE 500
#define NO_PWM 0

// 71 step / degree
// Gearbox de 47
#define STEP_PER_DEGREE ((int)(25600/360))
#define GEARBOX_RATIO (47)

void init_pwm(void) {
    // Set pwm pins
    gpio_set_function(pul_1_pin, GPIO_FUNC_PWM);
    gpio_set_function(pul_2_pin, GPIO_FUNC_PWM);
    gpio_set_function(pul_3_pin, GPIO_FUNC_PWM);
    gpio_set_function(pul_4_pin, GPIO_FUNC_PWM);

    // Get which pin is which slice
    uint8_t slice_1 = pwm_gpio_to_slice_num(pul_1_pin);
    uint8_t slice_2 = pwm_gpio_to_slice_num(pul_2_pin);
    uint8_t slice_3 = pwm_gpio_to_slice_num(pul_3_pin);
    uint8_t slice_4 = pwm_gpio_to_slice_num(pul_4_pin);

    // Get default config
    pwm_config config = pwm_get_default_config();
    // Set divider, reduces counter clock to 1 MHz
    pwm_config_set_clkdiv(&config, 125);

    // Load the configuration into our PWM slice and start the pwm
    pwm_init(slice_1, &config, true);
    pwm_init(slice_1, &config, true);
    pwm_init(slice_1, &config, true);
    pwm_init(slice_1, &config, true);

    // Set the max pwm duty cycle
    pwm_set_wrap(slice_1, MAX_DUTY_CYCLE);
    pwm_set_wrap(slice_2, MAX_DUTY_CYCLE);
    pwm_set_wrap(slice_3, MAX_DUTY_CYCLE);
    pwm_set_wrap(slice_4, MAX_DUTY_CYCLE);
}

int64_t stop_rotation(__unused alarm_id_t id, __unused void *user_data) {
    // Set all duty cycle to 0
    pwm_set_gpio_level(pul_1_pin, NO_PWM);
    pwm_set_gpio_level(pul_2_pin, NO_PWM);
    pwm_set_gpio_level(pul_3_pin, NO_PWM);
    pwm_set_gpio_level(pul_4_pin, NO_PWM);
    return 0;
}
void rotate_pv(uint16_t angle, bool clockwise) {
    // Enable motor
    if (clockwise) {
        // Set dir pin
    } else {
        // Set dir pin
    }
    
    // Start moving the motor
    pwm_set_gpio_level(pul_1_pin, DUTY_CYCLE);
    pwm_set_gpio_level(pul_2_pin, DUTY_CYCLE);
    pwm_set_gpio_level(pul_3_pin, DUTY_CYCLE);
    pwm_set_gpio_level(pul_4_pin, DUTY_CYCLE);

    // Set time to stop moving motor
    add_alarm_in_ms(angle*STEP_PER_DEGREE*GEARBOX_RATIO, stop_rotation, NULL, false);
}

