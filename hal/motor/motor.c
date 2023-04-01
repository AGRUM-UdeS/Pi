#include "motor.h"

#define pul_1_pin 12
#define pul_2_pin 13
#define pul_3_pin 14
#define pul_4_pin 15

#define ONE_KHZ 1000
#define DUTY_CYCLE 50
#define NO_PWM 0

// 71 step / degree
// Gearbox de 47
#define STEP_PER_DEGREE ((int)(25600/360))
#define GEARBOX_RATIO (47)

void init_motor(void) {
    // Set pwm pins
    init_pwm(pul_1_pin);
    init_pwm(pul_2_pin);
    init_pwm(pul_3_pin);
    init_pwm(pul_4_pin);

    // Set pwm frequency
    set_pwm_freq(pul_1_pin ,ONE_KHZ);
    set_pwm_freq(pul_2_pin ,ONE_KHZ);
    set_pwm_freq(pul_3_pin ,ONE_KHZ);
    set_pwm_freq(pul_4_pin ,ONE_KHZ);
}

int64_t stop_rotation(__unused alarm_id_t id, __unused void *user_data) {
    // Set all duty cycle to 0
    printf("Stop moving!\n");
    disable_pwm(pul_1_pin);
    disable_pwm(pul_2_pin);
    disable_pwm(pul_3_pin);
    disable_pwm(pul_4_pin);
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
    printf("Start moving!\n");
    enable_pwm(pul_1_pin, DUTY_CYCLE);
    enable_pwm(pul_2_pin, DUTY_CYCLE);
    enable_pwm(pul_3_pin, DUTY_CYCLE);
    enable_pwm(pul_4_pin, DUTY_CYCLE);

    // Set time to stop moving motor
    add_alarm_in_ms(angle*STEP_PER_DEGREE*GEARBOX_RATIO, stop_rotation, NULL, false);
}

