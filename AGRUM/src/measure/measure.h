#ifndef MEASURE_H
#define MEASURE_H

#include <stdio.h>
#include "pico/stdlib.h"

#define NUMBER_OF_LIMIT_SWITCH  8
#define NUMBER_OF_PV_ARRAY      2

typedef struct data_t {
  unsigned char* unit;
  float value;
} data_t;

typedef struct measure_t {
  data_t temperature;
  data_t humidity;
  uint limit_switch[NUMBER_OF_LIMIT_SWITCH];
  data_t voltage_pv[NUMBER_OF_PV_ARRAY];
  data_t current_pv[NUMBER_OF_PV_ARRAY];
} measure_t;

typedef enum {
  IDLE,
  MEASURING,
  MESURE_DONE,
  ERROR
} measure_state_t;

measure_state_t measure_sm(measure_t* measurements);

#endif
