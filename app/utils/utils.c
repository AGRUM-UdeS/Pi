/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "context.h"
#include "utils.h"

#define PING_PERIOD_MS  (1*60*1000)

static repeating_timer_t ping_timer;
static bool ping_interface_flag = false;

static bool pv_calibration_flag = false;
static bool weather_flag = false;
static bool irrigation_flag = false;

// Alarm once a day
static datetime_t morning_alarm = {
    .year  = -1,
    .month = -1,
    .day   = -1,
    .dotw  = -1,
    .hour  = 6,
    .min   = 0,
    .sec   = 0,
};

static void morning_alarm_cb(void)
{
    interface_publish("GOOD MORNING", 1);
    printf("Good morning!\n");
    pv_calibration_flag = true;
    weather_flag = true;
    irrigation_flag = true;
}

static bool ping_callback(repeating_timer_t *rt)
{
    ping_interface_flag = true;
    return ping_interface_flag;
}

void usb_delay(uint8_t delay_s)
{
    printf("\n");
    for (size_t i = 0; i <= delay_s; i++)
    {
        sleep_ms(1000);
        printf("%d..", (delay_s - i));
    }
    printf("\n");
}

void init_timing(void)
{
    
}

void init_hardware(void *ptr)
{
    main_context_t* context = (main_context_t*)ptr;

    init_i2c();

    // Open load relay as early as possible
    init_energy();

    // Init calibration button as input
    gpio_init(CALIBRATION_BUTTON);
    gpio_set_dir(CALIBRATION_BUTTON, GPIO_IN);
    if (!gpio_get(CALIBRATION_BUTTON)) {
        context->init_calib_pv = true;
    } else {
        context->init_calib_pv = false;
    }

    init_irrigation();

    init_motor();
}

#define HOUSEKEEPING_FREQUENCY_MS			( 1000 / portTICK_PERIOD_MS )

void house_keeping(void *pvParameters)
{
    init_watchdog();

    rtc_set_alarm(&morning_alarm, &morning_alarm_cb);

    // negative timeout means exact delay (rather than delay between callbacks)
    if (!add_repeating_timer_ms(-PING_PERIOD_MS, ping_callback, NULL, &ping_timer)) {
        interface_publish("GOOD MORNING", 0);
        printf("Failed to add ping timer\n");
    }

    interface_publish("Version logicielle", SW_VERSION);

    // init_heartbeat_led();

    while(1) {
        feed_watchdog();

        if (interface_is_connected() && ping_interface_flag) {
            // Send ping to the interface
            interface_publish(PI_STATUS_TOPIC, PI_STATUS_PING);
            vTaskDelay(500);
            datetime_t datetime;
            if (get_RTC_time(&datetime) != RTC_OK) {
                interface_publish("RTC hour", -1);
            } else {
                interface_publish("RTC hour", datetime.hour);
            }
            
            ping_interface_flag = false;
        }
        vTaskDelay(500);
    }
}

void clear_pv_calib_flag (void) {
    pv_calibration_flag = false;
}

bool morning_pv_calibration (void) {
    return pv_calibration_flag;
}

void clear_weather_flag (void) {
    weather_flag = false;
}

bool morning_weather(void) {
    return weather_flag;
}

void clear_irrigation_flag (void) {
    irrigation_flag = false;
}

bool morning_irrigation(void) {
    return irrigation_flag;
}

unsigned int countSetBits(unsigned int n)
{
    unsigned int count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}
