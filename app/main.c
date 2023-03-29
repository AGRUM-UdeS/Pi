/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"

void init(void) {
    // Init RP2040 peripherals
    stdio_init_all();

    // // Delay to let the developer open Putty
    sleep_ms(5000);

    /* Establish wifi connection
    SSID and Password are defined */
    printf("---------- Connecting to the wifi\n\n");
    // wifi_connect();

    // // Establish TCP/IP and MQTT connection
    // printf("\n-------- Establishing ThingsBoard connection\n");
    // ThingsBoard_connect();

    // // Getting date&time from an official server
    // // Then saving it into the RTC
    // printf("\n-------- Getting current date & time\n");
    // init_RTC();

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

        sleep_ms(100);

    }
    return 0;
}