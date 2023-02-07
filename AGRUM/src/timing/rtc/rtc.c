#include "rtc.h"

static bool is_init = false;

bool set_RTC_time(datetime_t** rtc_init_time) {
    // Initialize the real time clock
    printf("set_RTC_time 1\n");
    if (!is_init) {
        printf("set_RTC_time 2\n");
        rtc_init();
        is_init = true;
        printf("set_RTC_time 3\n");
    }
    while (!(rtc_running())) {
        tight_loop_contents();
    }
    printf("set_RTC_time 4\n");

    if (rtc_running()) {
        printf("set_RTC_time 5\n");
        bool ret = rtc_set_datetime(*rtc_init_time);
        // clk_sys is >2000x faster than clk_rtc, so datetime is not updated immediately when rtc_get_datetime() is called.
        // tbe delay is up to 3 RTC clock cycles (which is 64us with the default clock settings)
        sleep_us(64);
        printf("set_RTC_time 6\n");
        return ret;
    } else {
        printf("set_RTC_time 7\n");
        return false;
    }
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

