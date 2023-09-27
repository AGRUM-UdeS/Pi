#ifndef PV_MANAGEMENT_H
#define PV_MANAGEMENT_H

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"

#include "interface.h"
#include "sensors.h"

typedef enum _energy_status_t {
    ENERGY_INIT,
    ENERGY_IDLE,
    LOAD_SHEDDING,
    POWER_SAVING,
    NORMAL_USE,
    OVERCHARGED,
    ENERGY_ERROR
} energy_status_t;

#define NB_PV   2
#define NB_LIMIT_SWITCH     8
#define NB_BAT  2
#define LOAD_RELAY_GPIO     0

float get_instant_power_PV(void);

/*! \brief State-machine managing energy
 *
 *  Take care of energy related measurement
 *  as well as relays controlling the
 *  power flow.
 *
 * \return Last state of state-machine
 */
void enery_management(void *pvParameters);

#endif
