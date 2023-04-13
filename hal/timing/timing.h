#ifndef TIMING_H
#define TIMING_H

#include <stdint.h>
#include "interface.h"
#include "wrap_RTC.h"
#include "wrap_WATCHDOG.h"

typedef enum _timing_status_t{
    TIMING_OK,
    TIMING_FAILED
} timing_status_t;

/*! \brief Get date&time from the web and init real time clock
 *
 * \note Need wifi to do so
 * 
 * \return TIMING_OK if init was good, TIMING_FAILED if not
 */
uint8_t init_timer(void);

#endif