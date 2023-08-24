#ifndef IRRIGATION_H
#define IRRIGATION_H

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "hardware/rtc.h"
#include "pico/stdlib.h"
#include "pico/util/datetime.h"

#include "pump.h"
#include "valve.h"
#include "sensors.h"
#include "interface.h"

typedef enum _irrigation_status_t {
    IRRIGATION_INIT,
    IRRIGATION_IDLE,
    IRRIGATION_MEASUREMENT,
    IRRIGATION_WATERING,
    IRRIGATION_RESERVOIR2BARREL,
    IRRIGATION_SOAKING,
    IRRIGATION_ERROR
} irrigation_status_t;

/*! \brief Init irrigation hardware
 *
 */
void init_irrigation(void);

/*! \brief State-machine to deal with the irrigation
 *
 * \return Last state of state-machine
 */
void irrigation_management(void *pvParameters);

#endif
