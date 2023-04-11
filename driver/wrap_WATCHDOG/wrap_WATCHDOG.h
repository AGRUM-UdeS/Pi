#ifndef WRAP_WATCHDOG_H
#define WRAP_WATCHDOG_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/watchdog.h"

/*! \brief Init the watchdog peripheral
 */
void init_watchdog(void);

/*! \brief Update the watchdog so it doesn't reboot
 */
void feed_watchdog(void);

#endif
