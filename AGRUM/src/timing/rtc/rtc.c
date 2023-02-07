#include "rtc.h"

static bool is_init = false;

bool set_RTC_time(struct tm utc) {
    datetime_t* rtc_init_time;

    rtc_init_time->year  = utc.tm_year + 1900;
    rtc_init_time->month = utc.tm_mon + 1;
    rtc_init_time->day   = utc.tm_mday;
    rtc_init_time->dotw  = 0; // dont use day of the week
    rtc_init_time->hour  = utc.tm_hour;
    rtc_init_time->min   = utc.tm_min;
    rtc_init_time->sec   = utc.tm_sec;

    // Initialize the real time clock
    if (!is_init) {
        rtc_init();
        is_init = true;
    }

    bool ret = rtc_set_datetime(rtc_init_time);
    // clk_sys is >2000x faster than clk_rtc, so datetime is not updated immediately when rtc_get_datetime() is called.
    // tbe delay is up to 3 RTC clock cycles (which is 64us with the default clock settings)
    sleep_us(64);

    return ret;
}

bool get_RTC_time(datetime_t* datetime) {
    if (!is_init) {
        return false;
    } else {
        rtc_get_datetime(datetime);
        return true;
    }
}

bool is_RTC_init(void) {
    return is_init;
}

