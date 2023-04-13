#include "led.h"

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
