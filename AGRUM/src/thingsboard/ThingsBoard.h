#ifndef THINGSBOARD_H
#define THINGSBOARD_H

#include <stdbool.h>
#include <string.h>

#include "mqtt.h"
#include "measure.h"

typedef enum {
    THINGSBOARD_OK,
    THINGSBOARD_DISCONNECTED,
    THINGSBOARD_CONNECTING,
    THINGSBOARD_CONNECTED,
    THINGSBOARD_FAILED
} thingsboard_state_t;

thingsboard_state_t ThingsBoard_connect(void);
thingsboard_state_t ThingsBoard_publish(unsigned char* topic, float value);
thingsboard_state_t ThingsBoard_subscribe(void);
thingsboard_state_t ThingsBoard_unsubscribe(void);
thingsboard_state_t ThingsBoard_disconnect(void);
bool ThingsBoard_is_connected(void);

// The main state machine dealing with thingsboard
thingsboard_state_t thingsboard_sm(measure_t measurements);

#endif