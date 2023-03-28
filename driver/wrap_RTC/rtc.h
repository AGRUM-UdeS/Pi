#ifndef AGRUM_RTC_H
#define AGRUM_RTC_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "../ntp/ntp.h"

void init_RTC(void);
bool get_RTC_time(datetime_t* datetime);
bool RTC_initialized(void);

#endif