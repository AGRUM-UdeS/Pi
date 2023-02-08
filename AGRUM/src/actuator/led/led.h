#ifndef LED_H
#define LED_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

typedef enum {
  LED_ON,
  LED_OFF,
  LED_FLASHING
} led_state_t;

void init_heartbeat_led(void);

#endif