#ifndef MOTOR_H
#define MOTOR_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "wrap_PWM.h"

typedef enum motor_state_t {
  MOTOR_ON_CLOCKWISE,
  MOTOR_ON_COUNTERCLOCKWISE,
  MOTOR_OFF,
  MOTOR_CALIBRATING,
  MOTOR_UNKNOWN
} motor_state_t; 

void init_motor(void);
void rotate_pv(uint16_t angle, bool clockwise);

#endif