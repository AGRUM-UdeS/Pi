#ifndef PV_MANAGEMENT_H
#define PV_MANAGEMENT_H

#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"

typedef enum _energy_status_t {
    ENERGY_INIT,
    ENERGY_MEASUREMENT,
    LOAD_SHEDDING,
    POWER_SAVING,
    NORMAL_USE,
    OVERCHARGED,
    ENERGY_ERROR
} energy_status_t;

#define NB_PV   4

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
