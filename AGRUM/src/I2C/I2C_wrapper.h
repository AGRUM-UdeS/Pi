#ifndef I2C_WRAPPER_h
#define I2C_WRAPPER_h

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "PI4IOE5V9554.h"
#include "ADS7828.h"
#include "SHT.h"

void init_i2c(void);

#endif
