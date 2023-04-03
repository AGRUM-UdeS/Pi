#ifndef SENSORS_H
#define SENSORS_S

#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"

void init_water_level_sensors(void);

float get_PV_voltage(uint8_t PV_index);
float get_PV_current(uint8_t PV_index);

#endif