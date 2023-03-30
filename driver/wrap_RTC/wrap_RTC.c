#include "wrap_RTC.h"

#include "hardware/rtc.h"

#define CONNECTION_TIMEOUT_S 5

static bool is_init = false;

static rtc_status_t set_RTC_time(datetime_t rtc_time) {
    // Initialize the real time clock
    if (!is_init) {
        rtc_init();
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

bool RTC_initialized(void) {
    return is_init;
}

