/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "interface.h"
#include "utils.h"

void init(void) {
    // Init RP2040 peripherals
    stdio_init_all();

    // Delay to let the developer open Putty
    usb_delay();
    
    if (connect_to_interface() == INTERFACE_OK) {
        init_time();
    }

    // // Init I2C and assign right pins
    // init_i2c();

    // // Init PWM and assign right pins
    // init_pwm();

    // Get weather data
    // printf("\n-------- Getting weather data\n");
    // weather_current_request();
}

int main() {
    
    init();

    while (true) {
        printf("TEST\n");
        sleep_ms(1000);

    }
    return 0;
}
