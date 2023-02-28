#include "SHT.h"

const absolute_time_t ABSOLUTE_TIME_INITIALIZED_VAR(SHT_i2c_timeout, 10000); // 10 ms
#define STH1_BUF_LEN 4

SHT_status_t SHT3_read_temp_humidity(float* temp, float* humidity) {
    SHT_status_t rv = SHT_ok;

    return rv;
}

SHT_status_t SHT1_read_temp_humidity(float* temp, float* humidity) {
    SHT_status_t rv = SHT_ok;
    // Read temperature
    uint8_t raw_temp[STH1_BUF_LEN];
    i2c_read_blocking_until(i2c1, 0x03, raw_temp, (size_t)STH1_BUF_LEN, false, SHT_i2c_timeout);

    // Read humidity
    uint8_t raw_humidity[STH1_BUF_LEN];
    i2c_read_blocking_until(i2c1, 0x03, raw_humidity, (size_t)STH1_BUF_LEN, false, SHT_i2c_timeout);

    return rv;
}
