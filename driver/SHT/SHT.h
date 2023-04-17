#ifndef SHT_H
#define SHT_H

#include "pico/stdlib.h"

#include "wrap_I2C.h"

#define SHT3_address (0x44)
#define SHT1_address (0x00)

#define CLOCK_STRECHING           (0x2C)
#define HIGH_REPEATABILITY_CS     (0x06)
#define MEDIUM_REPEATABILITY_CS   (0x0D)
#define LOW_REPEATABILITY_CS      (0x10)

#define NO_CLOCK_STRECHING        (0x24)
#define HIGH_REPEATABILITY_NCS    (0x00)
#define MEDUIM_REPEATABILITY_NCS  (0x0B)
#define LOW_REPEATABILITY_NCS     (0x16)

#define SHT1_MEASURE_TEMPERATURE  (0x03)
#define SHT1_MEASURE_HUMIDITY     (0x05)

typedef struct _SHT_measure_t {
  float humidity;
  float temp;
  bool meas_ok;
} SHT_measure_t;

typedef enum _SHT_status_t{
  SHT_ok,
  SHT_timeout,
  SHT_error
} SHT_status_t;

/*! \brief Read SHT30 temperature and humidity
 *
 * \param temp float to store the converted temperature
 * \param humidity float to store the converted humdity
 * \return SHT_ok if everything good, SHT_error if other errors
 */
SHT_status_t SHT3_read_temp_humidity(float* temp, float* humidity, uint8_t i2c_bus);
// SHT_status_t SHT1_read_temp_humidity(float* temp, float* humidity);

#endif