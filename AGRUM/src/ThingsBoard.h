#ifndef THINGSBOARD_H
#define THINGSBOARD_H

#include <stdbool.h>
#include <string.h>

#include "mqtt.h"

typedef enum {
    THINGSBOARD_OK,
    THINGSBOARD_DISCONNECTED,
    THINGSBOARD_CONNECTING,
    THINGSBOARD_CONNECTED,
    THINGSBOARD_FAILED
} THINGSBOARD_STATUS;

THINGSBOARD_STATUS ThingsBoard_connect(void);
THINGSBOARD_STATUS ThingsBoard_publish(unsigned char* topic, float value);
THINGSBOARD_STATUS ThingsBoard_subscribe(void);
THINGSBOARD_STATUS ThingsBoard_unsubscribe(void);
THINGSBOARD_STATUS ThingsBoard_disconnect(void);
bool ThingsBoard_is_connected(void);

#endif