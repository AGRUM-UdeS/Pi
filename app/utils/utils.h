#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "wrap_I2C.h"

void usb_delay(uint8_t delay_s);
void init_peripherals(void);

#endif
