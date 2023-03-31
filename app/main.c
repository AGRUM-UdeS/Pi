/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "interface.h"
#include "PV_management.h"
#include "irrigation.h"
#include "utils.h"

void init(void) {
    // Init RP2040 peripherals
    stdio_init_all();

    // Delay to let the developer open Putty
    usb_delay(5);
    
    if (connect_to_interface() == INTERFACE_OK) {
        init_timer();
    }

    // Int everything irrigation related 
    init_irrigation();

    // Int everything solar panels related 
    init_PV();

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
