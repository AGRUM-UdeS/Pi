#ifndef AGRUM_RTC_H
#define AGRUM_RTC_H

#include "pico/stdlib.h"
#include "hardware/rtc.h"

void set_RTC_time(datetime_t** rtc_init_time);

bool get_RTC_time(datetime_t* datetime);

#endif