#ifndef TIMING_H
#define TIMING_H

#include <stdint.h>
#include "wrap_RTC.h"

typedef enum _timing_status_t{
    TIMING_OK,
    TIMING_FAILED
} timing_status_t;

uint8_t init_timer(void);

#endif