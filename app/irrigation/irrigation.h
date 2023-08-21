#ifndef IRRIGATION_H
#define IRRIGATION_H

#include "context.h"
#include "pump.h"
#include "valve.h"
#include "sensors.h"
#include "interface.h"

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
