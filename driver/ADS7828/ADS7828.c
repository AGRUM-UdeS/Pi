#include "ADS7828.h"

#define ADC_i2c_timeout make_timeout_time_ms(10)

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

ADC_status_t ADC_read_pin(uint8_t address, uint8_t pin_to_read, uint16_t* received_value)
{
    ADC_status_t status = ADC_ok;

    // Send the command byte to the ADC
    i2c0_write(address, &pin_to_read, (size_t)1);
    
    // Read the bytes from the ADC
    uint8_t received_byte[ADC_BUF_LEN];
    int nb = i2c0_read(address, received_byte, (size_t)ADC_BUF_LEN);

    *received_value = (((received_byte[0] & 0x0F) << 8) + (received_byte[1] & 0xFF));

    return return_ADC_status(nb);
}
