/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "wifi.h"
#include "ThingsBoard.h"
#include "actuator.h"
#include "measure.h"


void init(void) {
    // Init RP2040 peripherals
    stdio_init_all();

    // Delay to let the developer open Putty
    sleep_ms(5000);

    /* Establish wifi connection
    SSID and Password are defined */
    wifi_connect();

    // Establish TCP/IP and MQTT connection
    ThingsBoard_connect();
}

int main() {
    
    init();

    while (true) {
        static measure_t measurements;

        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(5000);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(5000);

        measure_sm(&measurements);

        actuator_sm(measurements);

        thingsboard_sm(measurements);
    }
    return 0;
}
