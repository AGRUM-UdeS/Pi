#ifndef INTTERFACE_H
#define INTTERFACE_H

typedef enum {
    WIFI_DISCONNECTED,
    WIFI_CONNECTING,
    WIFI_CONNECTED,
    WIFI_FAILED
} WIFI_STATUS;

WIFI_STATUS wifi_connect(void);

#endif