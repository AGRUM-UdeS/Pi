#ifndef ENERGY_MANAGEMENT_H
#define ENERGY_MANAGEMENT_H

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "motor.h"

typedef enum _PV_status_t {
    PV_INIT,
    PV_IDLE,
    PV_CALIBRATION,
    PV_DAYROTATION,
    PV_BADWEATHER,
    PV_BACKTRACKING,
    PV_ERROR
} PV_status_t;

void PV_management(void *pvParameters);

#endif
