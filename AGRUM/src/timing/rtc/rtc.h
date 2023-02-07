#ifndef AGRUM_RTC_H
#define AGRUM_RTC_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"
#include "ntp.h"

bool set_RTC_time(struct tm utc);

bool get_RTC_time(datetime_t* datetime);

bool is_RTC_init(void);

#endif