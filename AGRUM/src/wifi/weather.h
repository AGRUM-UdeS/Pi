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

void init_weather(void);

#endif
