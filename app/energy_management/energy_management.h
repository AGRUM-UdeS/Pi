#ifndef PV_MANAGEMENT_H
#define PV_MANAGEMENT_H

#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"

#define NB_PV   4

void init_energy_management(void);
float get_instant_power_PV(void);

#endif
