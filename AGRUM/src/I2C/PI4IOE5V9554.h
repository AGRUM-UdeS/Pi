#ifndef PI4IOE5V9554_H
#define PI4IOE5V9554_H

#include "hardware/i2c.h"
#include "pico/stdlib.h"

#define IO_address_0 (0x20)
#define IO_address_1 (0x21)
#define IO_address_2 (0x22)
#define IO_address_3 (0x23)
#define IO_address_4 (0x24)
#define IO_address_5 (0x25)
#define IO_address_6 (0x26)
#define IO_address_7 (0x27)

typedef enum _IO_status_t{
  IO_ok,
  IO_timeout,
  IO_error
} IO_status_t;

IO_status_t IO_read_pin(uint8_t address, uint8_t* received_data);
IO_status_t IO_write_pin(uint8_t address, uint8_t pin);

#endif
