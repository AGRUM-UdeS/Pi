#include "wrap_RTC.h"

#include "hardware/rtc.h"

#define CONNECTION_TIMEOUT_S 5

static bool is_init = false;

static rtc_status_t set_RTC_time(datetime_t rtc_time) {
    // Initialize the real time clock
    if (!is_init) {
        rtc_init();
        printf("rtc initialized\n");
        is_init = true;
    }

    if (!rtc_set_datetime(&rtc_time))
        return RTC_FAILED;

    // clk_sys is >2000x faster than clk_rtc, so datetime is not updated immediately when rtc_get_datetime() is called.
    // tbe delay is up to 3 RTC clock cycles (which is 64us with the default clock settings)
    sleep_us(64);
    return RTC_OK;
}

rtc_status_t init_RTC(datetime_t rtc_time) {
    if (set_RTC_time(rtc_time) != RTC_OK)
        return RTC_FAILED;

    return RTC_OK;
}

rtc_status_t get_RTC_time(datetime_t* datetime) {
    if (!(RTC_initialized())) {
        return RTC_FAILED;
    } else {
        rtc_get_datetime(datetime);
        return RTC_OK;
    }
}

rtc_status_t get_RTC_epoch_time(uint64_t* epoch_time) {
    datetime_t datetime;
    if (get_RTC_time(&datetime) != RTC_OK) {
        return RTC_FAILED;
    }
    struct tm t;
    time_t t_of_day;
    // Need local timezone
    t.tm_year = (datetime.year)-1900;  // Year - 1900
    t.tm_mon = (datetime.month) - 1;   // Month, where 0 = jan
    t.tm_mday = (datetime.day);        // Day of the month
    t.tm_hour = (datetime.hour);
    t.tm_min = (datetime.min);
    t.tm_sec = (datetime.sec);
    t.tm_isdst = -1; // Is DST on? 1 = yes, 0 = no, -1 = unknown
    t_of_day = mktime(&t);

    *epoch_time = (uint64_t)t_of_day;
    return RTC_OK;
}

bool RTC_initialized(void) {
    return is_init;
}

uint32_t epoch_to_hour(uint64_t epoch_time)
{
    uint32_t hour = (epoch_time / 3600) %  24;
    return hour;
}

uint32_t epoch_to_minute(uint64_t epoch_time)
{
    uint32_t minute = (epoch_time / 60) %  60;
    return minute;
}
