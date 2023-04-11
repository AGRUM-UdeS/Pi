#ifndef WRAP_WATCHDOG_H
#define WRAP_WATCHDOG_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/watchdog.h"


void init_watchdog(void);
void feed_watchdog(void);

#endif
