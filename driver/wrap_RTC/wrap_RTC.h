#ifndef AGRUM_RTC_H
#define AGRUM_RTC_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "wrap_NTP.h"

typedef enum _rtc_status_t{
    RTC_OK,
    RTC_FAILED
} rtc_status_t;

rtc_status_t init_RTC(datetime_t rtc_time);
rtc_status_t get_RTC_time(datetime_t* datetime);
bool RTC_initialized(void);

#endif