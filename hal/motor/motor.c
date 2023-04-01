#include "motor.h"

#define IO_MOTOR_ADDRESS   (IO_address_2)

#define PUL1_PIN    12
#define PUL2_PIN    13
#define PUL3_PIN    14
#define PUL4_PIN    15

#define DIR_1_PIN   7
#define DIR_2_PIN   5
#define DIR_3_PIN   2
#define DIR_4_PIN   0

#define ENA_1_PIN   6
#define ENA_2_PIN   4
#define ENA_3_PIN   3
#define ENA_4_PIN   1

#define ONE_KHZ 1000
#define DUTY_CYCLE 50
#define NO_PWM 0

// 71 step / degree
// Gearbox de 47
#define STEP_PER_DEGREE ((int)(25600/360))
#define GEARBOX_RATIO (47)

static void init_pul_pins(void)
{
    // Set pwm pins
    init_pwm(PUL1_PIN);
    init_pwm(PUL2_PIN);
    init_pwm(PUL3_PIN);
    init_pwm(PUL4_PIN);

    // Set pwm frequency
    set_pwm_freq(PUL1_PIN ,ONE_KHZ);
    set_pwm_freq(PUL2_PIN ,ONE_KHZ);
    set_pwm_freq(PUL3_PIN ,ONE_KHZ);
    set_pwm_freq(PUL4_PIN ,ONE_KHZ);
}

static void init_dir_pins(void)
{
    // Set dir pins as output on IO expander
    uint8_t byte = (1 << DIR_1_PIN) | (1 << DIR_2_PIN)
                 | (1 << DIR_3_PIN) | (1 << DIR_4_PIN);
    IO_set_as_output(IO_MOTOR_ADDRESS, byte);
}

static void init_ena_pins(void)
{
    // Clear pin before setting it as output
    // because default state is HIGH
    IO_clear_pin(IO_MOTOR_ADDRESS, ENA_1_PIN);
    IO_clear_pin(IO_MOTOR_ADDRESS, ENA_2_PIN);
    IO_clear_pin(IO_MOTOR_ADDRESS, ENA_3_PIN);
    IO_clear_pin(IO_MOTOR_ADDRESS, ENA_4_PIN);

    // Set enable pins as output on IO expander
    uint8_t byte = (1 << ENA_1_PIN) | (1 << ENA_2_PIN)
                 | (1 << ENA_3_PIN) | (1 << ENA_4_PIN);
    IO_set_as_output(IO_MOTOR_ADDRESS, byte);
}

void init_motor(void)
{
    // Init pins to control motor drives
    init_pul_pins();
    init_dir_pins();
    init_ena_pins();
}

int64_t stop_rotation(__unused alarm_id_t id, __unused void *user_data) {
    // Set all duty cycle to 0
    printf("Stop moving!\n");
    disable_pwm(PUL1_PIN);
    disable_pwm(PUL2_PIN);
    disable_pwm(PUL3_PIN);
    disable_pwm(PUL4_PIN);
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
    enable_pwm(PUL1_PIN, DUTY_CYCLE);
    enable_pwm(PUL2_PIN, DUTY_CYCLE);
    enable_pwm(PUL3_PIN, DUTY_CYCLE);
    enable_pwm(PUL4_PIN, DUTY_CYCLE);

    // Set time to stop moving motor
    add_alarm_in_ms(angle*STEP_PER_DEGREE*GEARBOX_RATIO, stop_rotation, NULL, false);
}

