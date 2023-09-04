#ifndef SENSORS_H
#define SENSORS_H

#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "ADS7828.h"
#include "SHT.h"

#define ENVIRO_SENSOR_NB    (4)

enum enviro_sensor_location_t {
    UNDER_PV_0,
    UNDER_PV_1,
    BESIDE_PV_0,
    BESIDE_PV_1
};

extern uint8_t enviro_sensor_location[ENVIRO_SENSOR_NB];

void init_water_level_sensors(void);

/*! \brief Read humidity and temperature sensor
 *
 * \return Struct containing temperature and humidity as float
 */
SHT_measure_t read_temp_humidity(uint8_t sensor);


float get_PV_voltage(uint8_t PV_index);
float get_PV_current(uint8_t PV_index);

#endif