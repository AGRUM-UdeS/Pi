#ifndef WRAP_PWM_H
#define WRAP_PWM_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

void init_pwm(uint8_t pin);
void set_pwm_freq(uint8_t pin, uint32_t freq);

#endif