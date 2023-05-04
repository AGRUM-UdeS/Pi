#ifndef WEATHER_H
#define WEATHER_H

#include <string.h>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "lwip/dns.h"
#include "lwipopts.h"

#include "lwip/apps/http_client.h"
#include "secret.h"

#define FORECAST_DAYS   3
#define FORECAST_HOURS  (FORECAST_DAYS*24)

typedef struct _weather_forecast_t {
    float precipitation_daily[FORECAST_DAYS];
    float max_windgust_daily[FORECAST_DAYS];
    uint16_t cloudcover_hourly[FORECAST_HOURS];
    uint64_t sunrise[FORECAST_DAYS];
    uint64_t sunset[FORECAST_DAYS];
    uint64_t last_updated;
} weather_forecast_t;

void weather_current_request(void);

#endif
