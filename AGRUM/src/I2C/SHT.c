#include "SHT.h"

const absolute_time_t ABSOLUTE_TIME_INITIALIZED_VAR(SHT_i2c_timeout, 10000); // 10 ms

SHT_status_t SHT3_read_temp_humidity(float* temp, float* humidity) {


    return rv;
}

SHT_status_t SHT1_read_temp_humidity(float* temp, float* humidity) {

    // Read temperature
    uint8_t* received_byte;
    i2c_read_blocking_until(i2c1, 0x03, received_byte, (size_t)4, false, SHT_i2c_timeout);

    return rv;
}
