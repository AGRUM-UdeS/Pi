#include "SHT.h"

// #define STH1_BUF_LEN 4
#define STH3_BUF_LEN 6

SHT_status_t SHT3_read_temp_humidity(float* temp, float* humidity) {
    SHT_status_t rv = SHT_ok;

    uint8_t buffer[] = {HIGH_REPEATABILITY_NCS, NO_CLOCK_STRECHING};
    if (i2c0_write(SHT3_address, buffer, sizeof(buffer)) != sizeof(buffer)) {
        rv = SHT_error;
    }
    sleep_ms(15); // Based on max measure time in datasheet
    uint8_t recept[STH3_BUF_LEN];
    if (i2c0_read(SHT3_address, recept, STH3_BUF_LEN) != STH3_BUF_LEN) {
        rv = SHT_error;
    }

    float cTemp = ((((recept[0] * 256.0) + recept[1]) * 175) / 65535.0) - 45;
    *temp = (cTemp * 1.8) + 32;
    *humidity = ((((recept[3] * 256.0) + recept[4]) * 100) / 65535.0);

    return rv;
}

// SHT_status_t SHT1_read_temp_humidity(float* temp, float* humidity) {
//     SHT_status_t rv = SHT_ok;
//     // Read temperature
//     uint8_t raw_temp[STH1_BUF_LEN];
//     i2c0_read(0x03, raw_temp, (size_t)STH1_BUF_LEN);

//     // Read humidity
//     uint8_t raw_humidity[STH1_BUF_LEN];
//     i2c0_read(0x03, raw_humidity, (size_t)STH1_BUF_LEN);

//     return rv;
// }
