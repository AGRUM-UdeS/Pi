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

uint8_t init_timer(void);
void house_keeping(bool init);

#endif