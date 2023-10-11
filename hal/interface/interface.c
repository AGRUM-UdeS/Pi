#include "interface.h"
#include "context.h"

main_context_t *context;

bool interface_publish(unsigned char *topic, float value) 
{
    if (!context) {
        return false;
    }
    mqtt_message_t msg;
    msg.value = value;
    snprintf(msg.topic, sizeof(msg.topic), "%s\0", topic);
    msg.topic[MAX_TOPIC_LEN - 1] = '\0';
    if (xQueueSend(context->interface_queue_handle, &msg, 500) == pdTRUE) {
        return true;
    }
    return false;
}

static bool interface_send(unsigned char *topic, float value)
{
    if (!interface_is_connected()) {
        return false;
    }
    
#ifdef PROD_BUILD
    if (ThingsBoard_publish(topic, value) != THINGSBOARD_OK) {
        printf("Client not connected...\n");
        return false;
    } else {
        uint32_t time_since_boot = to_ms_since_boot(get_absolute_time());
        printf("(%lu s) Value '%.2f' published to topic '%s'.\n", time_since_boot/1000, value, topic);
        return true;
    }
#else
    unsigned char test_topic[MAX_TOPIC_LEN];
    snprintf(test_topic, sizeof(test_topic), "TEST_%s\0", topic);
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

void interface(void *pvParameter)
{
    context = (main_context_t*)pvParameter; 
    context->interface_queue_handle = xQueueCreate(20, sizeof(mqtt_message_t));

    if (ThingsBoard_connect() == THINGSBOARD_CONNECTED) {
        context->interface_status = INTERFACE_CONNECTED;
        interface_send(PI_STATUS_TOPIC, PI_STATUS_CONNECTED);
    }

    while(true){
        // Make sure interface is up
        feed_watchdog();

        if (!interface_is_connected()) {
            printf("Interface disconnected, reconnecting\n");
            context->interface_status = INTERFACE_DISCONNECTED;
            if (ThingsBoard_connect() == THINGSBOARD_CONNECTED) {
                context->interface_status = INTERFACE_CONNECTED;
                interface_send(PI_STATUS_TOPIC, PI_STATUS_CONNECTED);
            } else {
                printf("Unable to connect to thingsboard, retrying...\n");
                vTaskDelay(500);
            }
            continue;
        } else {
            mqtt_message_t msg;
            if (xQueueReceive(context->interface_queue_handle, &msg, 200) == pdTRUE) {
                interface_send(msg.topic, msg.value);
            }
        }
        vTaskDelay(500);
    }
}
