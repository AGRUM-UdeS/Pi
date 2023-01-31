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

int main() {
    stdio_init_all();

    // Delay to let the developer open Putty
    sleep_ms(5000);

    wifi_connect();

    ThingsBoard_connect();

    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(5000);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(5000);

        ThingsBoard_publish("temperature", 4.20);
    }
    return 0;
}
