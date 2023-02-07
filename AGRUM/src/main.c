/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#define CYW43_ARCH_DEBUG_ENABLED 0

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "wifi.h"
#include "ThingsBoard.h"
#include "actuator.h"
#include "measure.h"
#include "ntp.h"


void heartbeat_led(void) {
    static bool led_state = false;
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_state);
    led_state = !led_state;
}

void init(void) {
    // Init RP2040 peripherals
    stdio_init_all();

    // Delay to let the developer open Putty
    sleep_ms(5000);

    /* Establish wifi connection
    SSID and Password are defined */
    wifi_connect();

    // Establish TCP/IP and MQTT connection
    printf("Establishing ThingsBoard connection...\n");
    ThingsBoard_connect();
    while (!(ThingsBoard_is_connected())) {
        tight_loop_contents();
    }

    printf("Getting current date & time...\n");
    get_time_ntp();
    while (!(is_RTC_init())) {
        tight_loop_contents();
    }


    // datetime_t t;
    // rtc_get_datetime(&t);
    // char datetime_buf[256];
    // char *datetime_str = &datetime_buf[0];
    // datetime_to_str(datetime_str, sizeof(datetime_buf), &t);
    // printf("\r%s      ", datetime_str);
}

int main() {
    
    init();

    while (true) {
        printf("LOOP\n");
        // static measure_t measurements;

        // measure_state_t measure_state = measure_sm(&measurements);

        // actuator_status_t actuator_status = actuator_sm(measurements, measure_state);

        // thingsboard_sm(measurements, actuator_status);
        sleep_ms(10);
    }
    return 0;
}
