#ifndef PUMP_H
#define PUMP_H

#include "wrap_I2C.h"

typedef enum pump_state_t{
  PUMP_ON,
  PUMP_OFF,
  PUMP_UNKNOWN
} pump_state_t; 

void init_pump(void);

#endif