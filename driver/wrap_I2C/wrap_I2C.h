#ifndef I2C_WRAPPER_h
#define I2C_WRAPPER_h

#include <stdio.h>
#include "pico/stdlib.h"

#include "hardware/i2c.h"

void init_i2c(void);
int32_t i2c0_write(uint8_t addr, const uint8_t *src, size_t len);
int32_t i2c0_read(uint8_t addr, uint8_t *dst, size_t len);

int32_t i2c1_write(uint8_t addr, const uint8_t *src, size_t len);
int32_t i2c1_read(uint8_t addr, uint8_t *dst, size_t len);


#endif
