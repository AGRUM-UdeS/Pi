#include "rtc.h"

static bool is_init = false;

void set_RTC_time(datetime_t** rtc_init_time) {
    // Initialize the real time clock
    if (!is_init) {
        rtc_init();
        is_init = true;
    }

    rtc_set_datetime(*rtc_init_time);
    // clk_sys is >2000x faster than clk_rtc, so datetime is not updated immediately when rtc_get_datetime() is called.
    // tbe delay is up to 3 RTC clock cycles (which is 64us with the default clock settings)
    sleep_us(64);
}

bool get_RTC_time(datetime_t* datetime) {
    if (!is_init) {
        return false;
    } else {
        rtc_get_datetime(datetime);
        return true;
    }
}