#include "timing.h"

#define SUNRISE_HOUR    (12)
#define SUNSET_HOUR     (13)

uint8_t init_timer(void)
{
    printf("\n----- Getting current date & time -----\n");
    // Get the time online
    datetime_t time;
    if (get_time_ntp(&time) != NTP_OK) {
        printf("failed ntp init\n");
        return TIMING_FAILED;
    }
    
    if (init_RTC(time) != RTC_OK) {
        printf("failed rtc init\n");
        return TIMING_FAILED;
    }

    return TIMING_OK;
}

bool daytime(void)
{
    datetime_t datetime;
    if (get_RTC_time(&datetime) != RTC_OK) {
        // Assume night time if rtc time invalid
        return false;
    }

    if (datetime.hour >= SUNRISE_HOUR && datetime.hour <= SUNSET_HOUR) {
        return true;
    }
}

bool morning(void)
{
    datetime_t datetime;
    if (get_RTC_time(&datetime) != RTC_OK) {
        // Assume night time if rtc time invalid
        return false;
    }

    if (datetime.hour == SUNRISE_HOUR) {
        return true;
    }
}
