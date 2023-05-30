#ifndef PV_MANAGEMENT_H
#define PV_MANAGEMENT_H

#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"

typedef enum _energy_status_t {
    ENERGY_INIT,
    ENERGY_IDLE,
    ENERGY_MEASUREMENT,
    INVERTER_DISCONNECT,
    INVERTER_CONNECT,
    BATTERY_DISCONNECT,
    BATTERY_CONNECT,
    LOAD_SHEDDING,
    ENERGY_ERROR
} energy_status_t;

#define NB_PV   4

void init_energy_management(void);
float get_instant_power_PV(void);

/*! \brief State-machine managing energy
 *
 *  Take care of energy related measurement
 *  as well as relays controlling the
 *  power flow.
 *
 * \return Last state of state-machine
 */
energy_status_t enery_management(void);

#endif
