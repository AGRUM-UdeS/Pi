#ifndef ADS7828_H
#define ADS7828_H

#include "pico/stdlib.h"

#include "wrap_I2C.h"

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
#define ADC_read_pin_0 (0x80)
#define ADC_read_pin_1 (0xC0)
#define ADC_read_pin_2 (0x90)
#define ADC_read_pin_3 (0xD0)
#define ADC_read_pin_4 (0xA0)
#define ADC_read_pin_5 (0xE0)
#define ADC_read_pin_6 (0xB0)
#define ADC_read_pin_7 (0xF0)

extern const uint16_t ADC_pin[8];

/*! \brief Read ADC raw value
 *
 * \param addr Address of the ADC to read from
 * \param pin_to_read Pin number to read the value from
 * \param received_value Pointer to store the received value
 * 
 * \return Number of bytes written, or 
 * PICO_ERROR_GENERIC if address not acknowledged, no device present, or 
 * PICO_ERROR_TIMEOUT if a timeout occurred.
 */
ADC_status_t ADC_read_pin(uint8_t addr, uint8_t pin_to_read, uint16_t* received_value);

ADC_status_t ADC1_read_pin(uint8_t addr, uint8_t pin_to_read, uint16_t* received_value);

/*! \brief Convert ADC raw value to a voltage
 *
 * \param bits 12 bits integer to convert
 * 
 * \return float voltage value
 */
float ADC_bits2voltage(uint16_t bits);

#endif
