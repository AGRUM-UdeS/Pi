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
#include "energy_management.h"
#include "utils.h"
#include "test.h"

#include "weather.h"

void init(void) {
    // Init RP2040 peripherals
    stdio_init_all();

    // Delay to let the developer open Putty
    usb_delay(5);

    // Init watchdog before wifi connection
    init_watchdog();
    
    if (connect_to_interface() == INTERFACE_CONNECTED) {
        init_timer();
    }

    feed_watchdog();

    init_peripherals();

    // Init everything irrigation related 
    init_irrigation();

    // Init everything solar panels related
    init_PV();

    // Init every thing energy related
    init_energy_management();

    // develop_test();
    feed_watchdog();

    // Get weather data
    // printf("\n-------- Getting weather data\n");
    // weather_current_request();
}

int main() {
    
    init();

    while (true) {
        house_keeping();
        interface_status_t status_interface = interface_sm();
        irrigation_status_t status_irrigation = irrigation_sm();
        send_system_status(status_interface, status_irrigation);
        //develop_test();
        sleep_ms(50);

    }
    return 0;
}
