#ifndef SHT_H
#define SHT_H

#include "pico/stdlib.h"

#define SHT3_address (0x44)
#define SHT1_address (0x00)

typedef enum _SHT_status_t{
  SHT_ok,
  SHT_timeout,
  SHT_error
} SHT_status_t;

SHT_status_t SHT3_read_temp_humidity(float* temp, float* humidity);
SHT_status_t SHT1_read_temp_humidity(float* temp, float* humidity);

#endif