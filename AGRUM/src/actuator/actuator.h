#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "measure.h"

#include "led.h"
#include "pump.h"
#include "valve.h"
#include "motor.h"


typedef struct actuator_state_t {
    led_state_t empty_barrel_led_state;
    led_state_t error_led_state;
    led_state_t measure_led_state;

    pump_state_t pump_state;
    valve_state_t valve_state;

    motor_state_t motor_state;
} actuator_state_t;

actuator_state_t actuator_sm(measure_t measurements);

#endif