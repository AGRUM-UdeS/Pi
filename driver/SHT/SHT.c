#include "SHT.h"

#define STH1_BUF_LEN 3
#define STH3_BUF_LEN 6

SHT_status_t SHT3_read_temp_humidity(float* temp, float* humidity, uint8_t i2c_bus) {
    SHT_status_t rv = SHT_ok;
    int32_t (*i2c_write)(uint8_t, const uint8_t*, size_t);
    int32_t (*i2c_read)(uint8_t, uint8_t*, size_t);

    if (i2c_bus == 0) {
        i2c_write = &i2c0_write;
        i2c_read = &i2c0_read;
    } else {
        i2c_write = &i2c1_write;
        i2c_read = &i2c1_read;
    }

    uint8_t buffer[] = {NO_CLOCK_STRECHING, HIGH_REPEATABILITY_NCS};
    if (i2c_write(SHT3_address, buffer, sizeof(buffer)) != sizeof(buffer)) {
        rv = SHT_error;
    }
    sleep_ms(15); // Based on max measure time in datasheet
    uint8_t recept[STH3_BUF_LEN];
    if (i2c_read(SHT3_address, recept, STH3_BUF_LEN) != STH3_BUF_LEN) {
        rv = SHT_error;
    }

    *temp = ((((recept[0] * 256.0) + recept[1]) * 175) / 65535.0) - 45;
    // float temp_f = (*temp * 1.8) + 32;
    *humidity = ((((recept[3] * 256.0) + recept[4]) * 100) / 65535.0);

    return rv;
}


SHT_status_t SHT1_read_temp_humidity(float* temp, float* humidity, uint8_t i2c_bus) {
    SHT_status_t rv = SHT_ok;
    int32_t (*i2c_write)(uint8_t, const uint8_t*, size_t);
    int32_t (*i2c_read)(uint8_t, uint8_t*, size_t);

    if (i2c_bus == 0) {
        i2c_write = &i2c0_write;
        i2c_read = &i2c0_read;
    } else {
        i2c_write = &i2c1_write;
        i2c_read = &i2c1_read;
    }

    // Temperature measurement
    if (i2c_write(SHT1_TEMP, NULL, 0) != 0) {
        rv = SHT_error;
    }
    sleep_ms(320); // Based on max measure time in datasheet
    uint8_t recept_temp[STH1_BUF_LEN];
    if (i2c_read(SHT1_TEMP, recept_temp, STH1_BUF_LEN) != STH1_BUF_LEN) {
        rv = SHT_error;
    }

    // TODO : validate conversion
    uint16_t raw_temp = ((recept_temp[0] << 8) & 0xff00)
                       + (recept_temp[1] & 0x00ff);
    *temp = -39.6 + 0.01 * raw_temp;

    // Humidity measurement
    if (i2c_write(SHT1_HUMIDITY, NULL, 0) != 0) {
        rv = SHT_error;
    }
    sleep_ms(320); // Based on max measure time in datasheet
    uint8_t recept_h[STH1_BUF_LEN];
    if (i2c_read(SHT1_HUMIDITY, recept_h, STH1_BUF_LEN) != STH1_BUF_LEN) {
        rv = SHT_error;
    }

    // TODO : validate conversion
    uint16_t raw_humi = ((recept_h[0] << 8) & 0xff00)
                       + (recept_h[1] & 0x00ff);
    *humidity = -2.0468 + (0.0367 * raw_humi) + (-0.0000015955 * raw_humi * raw_humi);

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
