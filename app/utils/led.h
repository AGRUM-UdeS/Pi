#ifndef LED_H
#define LED_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "PI4IOE5V9554A.h"

void init_heartbeat_led(void);

void init_irigation_led(void);

#endif
