#include "ADS7828.h"

const absolute_time_t ABSOLUTE_TIME_INITIALIZED_VAR(ADC_i2c_timeout, 10000); // 10 ms

static ADC_status_t return_ADC_status(int value){
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

ADC_status_t ADC_read_pin(uint8_t address, uint8_t pin_to_read, uint8_t* received_byte){
    ADC_status_t status = ADC_ok;

    // Send the command byte to the ADC
    i2c_write_blocking_until(i2c0, address, &pin_to_read, (size_t)1, false, ADC_i2c_timeout);
    
    // Read the bytes from the ADC
    int nb = i2c_read_blocking_until(i2c0, address, received_byte, (size_t)2, false, ADC_i2c_timeout);

    return return_ADC_status(nb);
}
