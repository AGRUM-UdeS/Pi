#ifndef VALVE_H
#define VALVE_H

#include "PI4IOE5V9554A.h"

typedef enum {
  VALVE_OPEN,
  VALVE_CLOSED,
  VALVE_UNKNOWN
} valve_state_t;

void init_valve(void);

#endif