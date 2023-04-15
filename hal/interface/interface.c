#include "interface.h"

bool interface_publish(unsigned char *topic, float value)
{
    if (ThingsBoard_publish(topic, value) != THINGSBOARD_OK) {
        return false;
    } else {
        return true;
    }
}

interface_status_t connect_to_interface(void)
{
    /* Establish wifi connection
    SSID and Password are defined in secret.c */
    if (wifi_connect() != WIFI_CONNECTED)
        return INTERFACE_DISCONNECTED;

    if (ThingsBoard_connect() != THINGSBOARD_CONNECTED)
        return INTERFACE_ERROR;

    ThingsBoard_publish(PI_STATUS_TOPIC, PI_STATUS_CONNECTED);

    return INTERFACE_CONNECTED;
}

bool interface_is_connected(void)
{
    return ThingsBoard_is_connected();
}

interface_status_t interface_sm(void)
{
    static interface_status_t interface_state = INTERFACE_CONNECTED;

    switch (interface_state) {
    case INTERFACE_CONNECTED:
        if (!interface_is_connected()) {
            interface_state = INTERFACE_DISCONNECTED;
        }

        break;

    case INTERFACE_CONNECTING:
        if (ThingsBoard_connect() == THINGSBOARD_CONNECTED) {
            interface_state = INTERFACE_CONNECTED;
        } else {
            printf("Unable to reconnect to thingsboard, retrying...\n");
        }
      
        break;

    case INTERFACE_DISCONNECTED:
        printf("Interface disconnected, reconnecting...\n");
        interface_state = INTERFACE_CONNECTING;

        break;

    case INTERFACE_ERROR:

        break;
    }


    return interface_state;
}
