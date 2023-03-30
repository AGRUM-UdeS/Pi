#include "timing.h"

uint8_t init_time(void) {
    printf("\n----- Getting current date & time -----\n");
    // Get the time online
    datetime_t time;
    if (get_time_ntp(&time) != NTP_OK)
        return TIMING_FAILED;
    
    if (init_RTC(time) != RTC_OK)
        return TIMING_FAILED;

    return TIMING_OK;
}
