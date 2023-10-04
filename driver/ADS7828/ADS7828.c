#include "ADS7828.h"

#define ADC_i2c_timeout make_timeout_time_ms(10)

const uint16_t ADC_pin[8] = {
  ADC_read_pin_0,
  ADC_read_pin_1,
  ADC_read_pin_2,
  ADC_read_pin_3,
  ADC_read_pin_4,
  ADC_read_pin_5,
  ADC_read_pin_6,
  ADC_read_pin_7,
};

static ADC_status_t return_ADC_status(int value)
{
    ADC_status_t rv;

    if (rv == PICO_ERROR_GENERIC) {
        rv = ADC_error;
    } else if (rv == PICO_ERROR_TIMEOUT) {
        rv = ADC_timeout;
    } else {
        rv = ADC_ok;
    }
    return rv;
}

ADC_status_t ADC_read_pin(uint8_t addr, uint8_t pin_to_read, uint16_t* received_value)
{
    ADC_status_t status = ADC_ok;
    uint8_t send_byte = ADC_pin[pin_to_read];

    // Send the command byte to the ADC
    i2c0_write(addr, &send_byte, (size_t)1);
    
    // Read the bytes from the ADC
    uint8_t received_byte[ADC_BUF_LEN];
    int nb = i2c0_read(addr, received_byte, (size_t)ADC_BUF_LEN);

    *received_value = (((received_byte[0] & 0x0F) << 8) + (received_byte[1] & 0xFF));

    return return_ADC_status(nb);
}

ADC_status_t ADC1_read_pin(uint8_t addr, uint8_t pin_to_read, uint16_t* received_value)
{
    ADC_status_t status = ADC_ok;
    uint8_t send_byte = ADC_pin[pin_to_read];

    // Send the command byte to the ADC
    i2c1_write(addr, &send_byte, (size_t)1);
    
    // Read the bytes from the ADC
    uint8_t received_byte[ADC_BUF_LEN];
    int nb = i2c1_read(addr, received_byte, (size_t)ADC_BUF_LEN);

    *received_value = (((received_byte[0] & 0x0F) << 8) + (received_byte[1] & 0xFF));

    return return_ADC_status(nb);
}

float ADC_bits2voltage(uint16_t bits)
{
    return ((float)bits * 5.0 / 4096);
}
