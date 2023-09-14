#ifndef LED_H
#define LED_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "PI4IOE5V9554A.h"

void init_heartbeat_led(void);

void clear_all_irigation_led(void);

void turn_on_irrigation_led(uint8_t del_id);

void turn_off_irrigation_led(uint8_t del_id);

#endif
