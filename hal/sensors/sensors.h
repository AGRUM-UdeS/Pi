#ifndef SENSORS_H
#define SENSORS_S

#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "SHT.h"

void init_water_level_sensors(void);

/*! \brief Read humidity and temperature sensor
 *
 * \return Struct containing temperature and humidity as float
 */
SHT_measure_t read_temp_humidity(void);

float get_PV_voltage(uint8_t PV_index);
float get_PV_current(uint8_t PV_index);

#endif