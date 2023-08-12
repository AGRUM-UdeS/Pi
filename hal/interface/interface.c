#include "interface.h"

// Uncomment this for prod build
// #define PUBLISH

bool interface_publish(unsigned char *topic, float value)
{
    if (!interface_is_connected()) {
        return false;
    }
    
#ifdef PUBLISH
    sleep_ms(5); // Delay not to overload wifi
    if (ThingsBoard_publish(topic, value) != THINGSBOARD_OK) {
        printf("Client not connected...\n");
        return false;
    } else {
        uint32_t time_since_boot = to_ms_since_boot(get_absolute_time());
        printf("(%lu s) Value '%.2f' published to topic '%s'.\n", time_since_boot/1000, value, topic);
        return true;
    }
#else
    unsigned char test_topic[30];
    snprintf(test_topic, sizeof(test_topic), "TEST_%s", topic);
    if (ThingsBoard_publish(test_topic, value) != THINGSBOARD_OK) {
        printf("Client not connected...\n");
        return false;
    } else {
        uint32_t time_since_boot = to_ms_since_boot(get_absolute_time());
        printf("(%lu s) Value '%.2f' published to topic '%s'.\n", time_since_boot/1000, value, test_topic);
        return true;
    }
    return true;
#endif
}

interface_status_t connect_to_interface(void)
{
    /* Establish wifi connection
    SSID and Password are defined in secret.c */
    if (wifi_connect() != WIFI_CONNECTED)
        return INTERFACE_DISCONNECTED;

    return INTERFACE_CONNECTED;
}

bool interface_is_connected(void)
{
    return ThingsBoard_is_connected();
}

void interface(void *pvParameters)
{
    interface_status_t *interface_state = (interface_status_t*)pvParameters;
    *interface_state = INTERFACE_CONNECTING;

    while (1) {

        switch (*interface_state) {
        case INTERFACE_CONNECTED:
            if (!interface_is_connected()) {
                *interface_state = INTERFACE_DISCONNECTED;
            }

            break;

        case INTERFACE_CONNECTING:
            if (ThingsBoard_connect() == THINGSBOARD_CONNECTED) {
                interface_publish(PI_STATUS_TOPIC, PI_STATUS_CONNECTED);
                *interface_state = INTERFACE_CONNECTED;
            } else {
                printf("Unable to reconnect to thingsboard, retrying...\n");
            }
        
            break;

        case INTERFACE_DISCONNECTED:
            printf("Interface disconnected, reconnecting...\n");
            *interface_state = INTERFACE_CONNECTING;

            break;

        case INTERFACE_ERROR:

            break;
        }
        vTaskDelay(10);
    }
}
