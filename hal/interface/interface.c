#include "interface.h"

interface_status_t connect_to_interface(void)
{
    /* Establish wifi connection
    SSID and Password are defined in secret.c */
    if (wifi_connect() != WIFI_CONNECTED)
        return INTERFACE_DISCONNECTED;

    if (ThingsBoard_connect() != THINGSBOARD_CONNECTED)
        return INTERFACE_ERROR;

    return INTERFACE_OK;
}

bool interface_is_connected(void)
{
    return ThingsBoard_is_connected();
}
