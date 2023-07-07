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
#include "wrap_RTC.h"
#include "interface.h"

#include "hardware/rtc.h"
#include "pico/stdlib.h"
#include "pico/util/datetime.h"

#define FORECAST_DAYS   3
#define FORECAST_HOURS  (FORECAST_DAYS*24)

#define PRECIPITATION_TOPIC     ("Precipitation day")
#define WINDGUST_TOPIC          ("Windgust day")
#define SUNRISE_HOUR_TOPIC      ("Sunrise hour day")
#define SUNRISE_MINUTE_TOPIC    ("Sunrise minute day")
#define SUNSET_HOUR_TOPIC       ("Sunset hour day")
#define SUNSET_MINUTE_TOPIC     ("Sunset minute day")

typedef enum _weather_status_t {
    WEATHER_OK,
    WEATHER_ERROR
} weather_status_t;

typedef struct _weather_forecast_t {
    float precipitation_daily[FORECAST_DAYS];
    float max_windgust_daily[FORECAST_DAYS];
    uint16_t cloudcover_hourly[FORECAST_HOURS];
    uint64_t sunrise[FORECAST_DAYS];
    uint64_t sunset[FORECAST_DAYS];
    uint64_t last_updated;
} weather_forecast_t;

typedef struct _weather_handle_t {
    bool is_received;
    weather_forecast_t weather_forecast;
} weather_handle_t;

typedef enum _weather_print_log_t {
    NO_LOG_NO_PRINT_WEATHER,
    PRINT_WEATHER,
    LOG_WEATHER,
    LOG_AND_PRINT_WEATHER
} weather_print_log_t;

void weather_init(weather_handle_t* weather_handle);
weather_status_t weather_task(weather_handle_t* weather_handle);
void weather_printf(weather_handle_t* weather_handle, weather_print_log_t print_log);

#endif
