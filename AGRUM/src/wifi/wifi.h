#ifndef WIFI_H
#define WIFI_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "secret.h"

typedef enum {
    WIFI_DISCONNECTED,
    WIFI_CONNECTING,
    WIFI_CONNECTED,
    WIFI_FAILED
} WIFI_STATUS;

WIFI_STATUS wifi_connect(void);

#endif