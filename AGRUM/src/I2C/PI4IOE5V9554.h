#ifndef PI4IOE5V9554_H
#define PI4IOE5V9554_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>

#define IO_address_0      (0x38)
#define IO_address_1      (0x39)
#define IO_address_2      (0x3A)
#define IO_address_3      (0x3B)
#define IO_address_4      (0x3C)
#define IO_address_5      (0x3D)
#define IO_address_6      (0x3E)
#define IO_address_7      (0x3F)

#define IO_READ           (0x0)
#define IO_WRITE          (0x1)
#define IO_POLARITY_INV   (0x2)
#define IO_PIN_DIR        (0x3)

#define IO_MASK_1         (0x01)
#define IO_MASK_2         (0x02)
#define IO_MASK_3         (0x04)
#define IO_MASK_4         (0x08)
#define IO_MASK_5         (0x10)
#define IO_MASK_6         (0x20)
#define IO_MASK_7         (0x40)
#define IO_MASK_8         (0x80)

typedef enum _IO_status_t{
  IO_ok,
  IO_timeout,
  IO_error
} IO_status_t;

IO_status_t IO_read_port(uint8_t address, uint8_t* received_data);
IO_status_t IO_write_pin(uint8_t address, uint8_t pin);

#endif
