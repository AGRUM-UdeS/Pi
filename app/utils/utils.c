#include "utils.h"

#define PING_PERIOD_MS  30000

static repeating_timer_t ping_timer;
static bool ping_interface_flag = false;

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

void init_peripherals(void)
{
    init_i2c();
    init_watchdog();
    // negative timeout means exact delay (rather than delay between callbacks)
    if (!add_repeating_timer_ms(-PING_PERIOD_MS, ping_callback, NULL, &ping_timer)) {
        printf("Failed to add ping timer\n");
    }
    // init_heartbeat_led();
}

void house_keeping(void)
{
    feed_watchdog();

    if (interface_is_connected() && ping_interface_flag) {
        // Send ping to the interface
        interface_publish(PI_STATUS_TOPIC, PI_STATUS_PING);
        ping_interface_flag = false;
    }
}

void send_system_status(
    interface_status_t status_interface,
    irrigation_status_t status_irrigation)
{
    if (interface_is_connected()) {
        static system_status_t last_status = SYSTEM_ERROR;
        system_status_t status = SYSTEM_IDLE;
        // TODO: Add more states
        if (status_irrigation == IRRIGATION_MEASURING) {
            status = SYSTEM_MEASURING;
        } else if (status_irrigation == IRRIGATION_IRRIGATING) {
            status = SYSTEM_IRRIGATING;
        } else if (status_irrigation == IRRIGATION_PUMPING) {
            status = SYSTEM_WATER_PUMPING;
        }

        // Publish only on system states changes
        if (last_status != status) {
            if (interface_publish(SYSTEM_STATUS_TOPIC, status)) {
                last_status = status;
            }
        }
    } else {
        // Turn on error LED
    }
}
