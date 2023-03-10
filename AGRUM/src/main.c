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
#include "ntp.h"
#include "I2C_wrapper.h"


void init(void) {
    // Init RP2040 peripherals
    stdio_init_all();

    // Delay to let the developer open Putty
    sleep_ms(5000);

    /* Establish wifi connection
    SSID and Password are defined */
    printf("---------- Connecting to the wifi\n\n");
    wifi_connect();

    // Establish TCP/IP and MQTT connection
    printf("\n-------- Establishing ThingsBoard connection\n");
    ThingsBoard_connect();

    // Getting date&time from an official server
    // Then saving it into the RTC
    printf("\n-------- Getting current date & time\n");
    init_RTC();

    // Init I2C and assign right pins
    init_i2c();
}

int main() {
    
    init();

    while (true) {

        static measure_t measurements;

        measure_state_t measure_state = measure_sm(&measurements);

        actuator_status_t actuator_status = actuator_sm(measurements, measure_state);

        thingsboard_sm(measurements, actuator_status);
        sleep_ms(100);
    }
    return 0;
}
