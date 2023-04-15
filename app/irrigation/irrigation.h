#ifndef IRRIGATION_H
#define IRRIGATION_H

#include "pump.h"
#include "valve.h"
#include "sensors.h"
#include "interface.h"

typedef enum _irrigation_status_t{
    IRRIGATION_IDLE,
    IRRIGATION_MEASURING,
    IRRIGATION_PUMPING,
    IRRIGATION_IRRIGATING,
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
irrigation_status_t irrigation_sm(void);

#endif
