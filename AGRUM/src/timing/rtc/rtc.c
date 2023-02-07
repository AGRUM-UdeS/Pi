#include "rtc.h"

static bool is_init = false;

static datetime_t utc_to_datetime(struct tm utc) {
    datetime_t datetime;
    
    datetime.year  = utc.tm_year + 1900;
    datetime.month = utc.tm_mon + 1;
    datetime.day   = utc.tm_mday;
    datetime.dotw  = utc.tm_wday;
    datetime.hour  = utc.tm_hour;
    datetime.min   = utc.tm_min;
    datetime.sec   = utc.tm_sec;

    return datetime;
}

static void set_RTC_time(datetime_t rtc_time) {
    // Initialize the real time clock
    if (!is_init) {
        rtc_init();
        is_init = true;
    }

    rtc_set_datetime(&rtc_time);

    // clk_sys is >2000x faster than clk_rtc, so datetime is not updated immediately when rtc_get_datetime() is called.
    // tbe delay is up to 3 RTC clock cycles (which is 64us with the default clock settings)
    sleep_us(64);
}

void init_RTC(void) {
    get_time_ntp();
    while (!(ntp_time_received())) {
        tight_loop_contents();
    }

    set_RTC_time(utc_to_datetime(get_utc()));
}

bool get_RTC_time(datetime_t* datetime) {
    if (!(RTC_initialized())) {
        return false;
    } else {
        rtc_get_datetime(datetime);
        return true;
    }
}

bool RTC_initialized(void) {
    return is_init;
}

