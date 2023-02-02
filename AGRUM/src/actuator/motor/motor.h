#ifndef MOTOR_H
#define MOTOR_H

typedef enum motor_state_t {
  MOTOR_ON_CLOCKWISE,
  MOTOR_ON_COUNTERCLOCKWISE,
  MOTOR_OFF,
  MOTOR_CALIBRATING,
  MOTOR_UNKNOWN
} motor_state_t; 

#endif