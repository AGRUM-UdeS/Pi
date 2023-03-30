#include "timing.h"

uint8_t init_time(void) {
    // Get the time online
    datetime_t time;
    if (get_time_ntp(&time) != NTP_OK)
        return -1;
    
    if (init_RTC(time) != RTC_OK)
        return -1;

    return 0;
}
