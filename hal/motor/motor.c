#include "motor.h"

#define IO_MOTOR_ADDRESS   (IO_address_2)

#define MOTOR_NUM    4

#define PUL_1_PIN    12
#define PUL_2_PIN    13
#define PUL_3_PIN    14
#define PUL_4_PIN    15

const uint8_t PUL_PIN[] = {
    PUL_1_PIN,
    PUL_2_PIN,
    PUL_3_PIN,
    PUL_4_PIN
};

#define DIR_1_PIN   7
#define DIR_2_PIN   5
#define DIR_3_PIN   2
#define DIR_4_PIN   0

const uint8_t DIR_PIN[] = {
    DIR_1_PIN,
    DIR_2_PIN,
    DIR_3_PIN,
    DIR_4_PIN
};

#define ENA_1_PIN   6
#define ENA_2_PIN   4
#define ENA_3_PIN   3
#define ENA_4_PIN   1

const uint8_t ENA_PIN[] = {
    ENA_1_PIN,
    ENA_2_PIN,
    ENA_3_PIN,
    ENA_4_PIN
};

static uint8_t motor_to_stop[MOTOR_NUM];

#define ONE_KHZ 1000
#define DUTY_CYCLE 50
#define NO_PWM 0

// 71 step / degree
// Gearbox de 47
#define STEP_PER_DEGREE ((int)(25600/360))
#define GEARBOX_RATIO (47)

static motor_state_t init_pul_pins(void)
{
    for (size_t i = 0; i < sizeof(MOTOR_NUM); i++) {
        // Set pwm pins
        init_pwm(PUL_PIN[i]);
        // Set pwm frequency
        set_pwm_freq(PUL_PIN[i] ,ONE_KHZ);
    }
    return MOTOR_OK;
}

static motor_state_t init_dir_pins(void)
{
    motor_state_t status = MOTOR_OK;

    for (size_t i = 0; i < sizeof(MOTOR_NUM); i++) {
        // No need to init a state here, but hey, we're safe
        if (IO_clear_pin(IO_MOTOR_ADDRESS, DIR_PIN[i]) != IO_ok){
            status = MOTOR_ERROR;
        }

        // Set dir pins as output on IO expander
        if (IO_set_as_output(IO_MOTOR_ADDRESS, DIR_PIN[i]) != IO_ok){
            status = MOTOR_ERROR;
        }
    }
    return status;
}

static motor_state_t init_ena_pins(void)
{
    motor_state_t status = MOTOR_OK;

    for (size_t i = 0; i < sizeof(MOTOR_NUM); i++) {
        // Make sure motor drives are enabled
        if (IO_set_pin(IO_MOTOR_ADDRESS, ENA_PIN[i]) != IO_ok){
            status = MOTOR_ERROR;
        }

        // Set enable pins as output on IO expander
        if (IO_set_as_output(IO_MOTOR_ADDRESS, ENA_PIN[i]) != IO_ok){
            status = MOTOR_ERROR;
        }
    }
    return status;
}

motor_state_t init_motor(void)
{
    motor_state_t status = MOTOR_OK;
    // Init pins to control motor drives
    if (init_pul_pins() != MOTOR_OK) {
        status = MOTOR_ERROR;
    }
    if (init_dir_pins() != MOTOR_OK) {
        status = MOTOR_ERROR;
    }
    if (init_ena_pins() != MOTOR_OK) {
        status = MOTOR_ERROR;
    }
    return status;
}

static int64_t stop_rotation(__unused alarm_id_t id, void *user_data)
{
    if (user_data == NULL) {
        // Set all duty cycle to 0
        printf("Stop moving (all)!\n");
        for (size_t i = 0; i < sizeof(MOTOR_NUM); i++) {
            disable_pwm(PUL_PIN[i]);
        }
    } else {
        // Tell the code user_data is a uint8_t ptr and dereference it
        uint8_t motor_index = *((uint8_t*)(user_data));
        printf("Stop moving (%d)!\n", motor_index);
        disable_pwm(PUL_PIN[motor_index]);
    }

    return 0;
}

motor_state_t rotate_all_pv(uint16_t angle, bool clockwise)
{
    if (clockwise) {
        for (size_t i = 0; i < sizeof(MOTOR_NUM); i++) {
            IO_clear_pin(IO_MOTOR_ADDRESS, DIR_PIN[i]);
        }
    } else {
        for (size_t i = 0; i < sizeof(MOTOR_NUM); i++) {
            IO_set_pin(IO_MOTOR_ADDRESS, DIR_PIN[i]);
        }
    }
    
    // Start moving the motor
    printf("Start moving (all)!\n");
    for (size_t i = 0; i < sizeof(MOTOR_NUM); i++) {
        enable_pwm(PUL_PIN[i], DUTY_CYCLE);
    }

    // Set time to stop moving motor
    add_alarm_in_ms(angle*STEP_PER_DEGREE*GEARBOX_RATIO, stop_rotation, NULL, false);
}

motor_state_t rotate_single_pv(uint8_t ind_motor, uint16_t angle, bool clockwise)
{
    if (ind_motor >= MOTOR_NUM) {
        printf("Wrong motor index (%d)", ind_motor);
        return MOTOR_ERROR;
    }

    motor_state_t status = MOTOR_OK;
    if (clockwise) 
    {
        if (IO_clear_pin(IO_MOTOR_ADDRESS, DIR_PIN[ind_motor]) != IO_ok) {
            status = MOTOR_ERROR;
        }
    } 
    else 
    {
        if (IO_set_pin(IO_MOTOR_ADDRESS, DIR_PIN[ind_motor]) != IO_ok) {
            status = MOTOR_ERROR;
        }
    }

    // Start moving the motor
    printf("Start moving (%d)!\n", ind_motor);
    enable_pwm(PUL_PIN[ind_motor], DUTY_CYCLE);

    // Set time to stop moving motor
    motor_to_stop[ind_motor] = ind_motor;
    add_alarm_in_ms(angle*STEP_PER_DEGREE*GEARBOX_RATIO, stop_rotation, &(motor_to_stop[ind_motor]), false);
    
    return status;
}

