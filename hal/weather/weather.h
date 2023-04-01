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

void weather_forecast_request(void);
void weather_current_request(void);
void print_weather_forecast(void);
void print_current_weather(void);

#endif