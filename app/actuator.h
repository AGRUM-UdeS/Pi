#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "../measure/measure.h"

#include "led/led.h"
#include "pump/pump.h"
#include "valve/valve.h"
#include "motor/motor.h"

typedef enum _actuator_state_t{
  ACTUATOR_INIT,
  ACTUATOR_IDLE,
  MOVING_MOTOR,
  CALIBRATING_MOTOR,
  PUMPING_WATER,
  IRRIGATING,
  ACTUATOR_ERROR
} actuator_state_t;


typedef struct actuator_status_t {
    led_state_t empty_barrel_led_state;
    led_state_t error_led_state;
    led_state_t measure_led_state;

    pump_state_t pump_state;
    valve_state_t valve_state;

    motor_state_t motor_state;
} actuator_status_t;

actuator_status_t actuator_sm(measure_t measurements, const measure_state_t measure_state);

#endif