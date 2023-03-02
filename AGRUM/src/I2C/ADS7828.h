#ifndef ADS7828_H
#define ADS7828_H

#include "hardware/i2c.h"
#include "pico/stdlib.h"

#define ADC_BUF_LEN 2

typedef enum _ADC_status_t{
  ADC_ok,
  ADC_timeout,
  ADC_error
} ADC_status_t;

// Chip address
#define ADC_address_0 (0x48)
#define ADC_address_1 (0x49)
#define ADC_address_2 (0x4A)
#define ADC_address_3 (0x4B)

// Command byte
// Single ended and power down between conversion
#define ADC_read_pin_1 (0x80)
#define ADC_read_pin_2 (0x90)
#define ADC_read_pin_3 (0xA0)
#define ADC_read_pin_4 (0xB0)
#define ADC_read_pin_5 (0xC0)
#define ADC_read_pin_6 (0xD0)
#define ADC_read_pin_7 (0xE0)
#define ADC_read_pin_8 (0xF0)

ADC_status_t ADC_read_pin(uint8_t address, uint8_t pin_to_read, uint8_t* received_byte);

#endif
