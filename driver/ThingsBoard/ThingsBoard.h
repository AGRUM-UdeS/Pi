#ifndef THINGSBOARD_H
#define THINGSBOARD_H

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "wrap_WATCHDOG.h"
#include "wrap_RTC.h"
#include "secret.h"

#include "wrap_MQTT.h"

#define MAX_TOPIC_LEN           (64)

typedef enum _thingsboard_state_t{
    THINGSBOARD_IDLE,
    THINGSBOARD_SEND_DATA,
    THINGSBOARD_READ_DATA,
    THINGSBOARD_CONNECTING,
    THINGSBOARD_ERROR
} thingsboard_state_t;

typedef enum _thingsboard_status_t{
    THINGSBOARD_OK,
    THINGSBOARD_DISCONNECTED,
    THINGSBOARD_CONNECTED,
    THINGSBOARD_FAILED
} thingsboard_status_t;

thingsboard_state_t ThingsBoard_connect(void);
thingsboard_state_t ThingsBoard_publish(unsigned char* topic, float value);
thingsboard_state_t ThingsBoard_subscribe(void);
thingsboard_state_t ThingsBoard_unsubscribe(void);
thingsboard_state_t ThingsBoard_disconnect(void);
bool ThingsBoard_is_connected(void);

// The main state machine dealing with thingsboard
thingsboard_state_t thingsboard_sm(void);

#endif