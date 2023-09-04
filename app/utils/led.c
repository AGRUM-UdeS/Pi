#include "led.h"

#define IO_IRRIGATION_ADDRESS   (IO_address_0)
#define LED_BARIL               (6)
#define LED_BAC                 (7)

#define HEARTBEAT_LED_PERIOD_US (-500 * 1000)

repeating_timer_t heartbeat_led_timer;

static bool heartbeat_led(repeating_timer_t *rt) {
    static bool heartbeat_led_state = false;
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, heartbeat_led_state);
    heartbeat_led_state = !heartbeat_led_state;
    return true;
}

void init_heartbeat_led(void) {
    // negative timeout means exact delay (rather than delay between callbacks)
    if (!add_repeating_timer_us(HEARTBEAT_LED_PERIOD_US, heartbeat_led, NULL, &heartbeat_led_timer)) {
        printf("Failed to init heartbeat led\n");
    }
}

void init_irigation_led(void)
{
    // Clear led baril
    IO_clear_pin(IO_IRRIGATION_ADDRESS, LED_BARIL);
    IO_set_pin(IO_IRRIGATION_ADDRESS, LED_BARIL);

    // Clear led bac
    IO_clear_pin(IO_IRRIGATION_ADDRESS, LED_BAC);
    IO_set_pin(IO_IRRIGATION_ADDRESS, LED_BAC);
}
