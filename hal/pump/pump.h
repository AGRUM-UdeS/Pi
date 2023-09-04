#ifndef PUMP_H
#define PUMP_H

#include "PI4IOE5V9554A.h"

typedef enum pump_state_t{
  PUMP_ON,
  PUMP_OFF,
  PUMP_UNKNOWN
} pump_state_t; 

void disable_all_pump(void);

#endif