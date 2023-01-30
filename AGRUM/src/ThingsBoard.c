
#include "ThingsBoard.h"

static mqtt_client_t* mqtt_client;

#define KEEP_ALIVE_TIMEOUT 3600
#define THINGSBOARD_HOSTNAME "thingsboard.cloud"

static const struct mqtt_connect_client_info_t mqtt_client_info =
{
  "RaspberryPiPicoW", /* Client id */
  "RaspberryPiPicoW", /* user, or access token in our case */
  "RaspberryPiPicoW", /* pass */
  KEEP_ALIVE_TIMEOUT,  /* keep alive */
  NULL, /* will_topic */
  NULL, /* will_msg */
  0,    /* will_qos */
  0     /* will_retain */
};


THINGSBOARD_STATUS ThingsBoard_connect(void) {
  printf("Trying to connect to thingsboard...\n");
  err_t connect_status = mqtt_connect(mqtt_client, &mqtt_client_info, THINGSBOARD_HOSTNAME);
  // printf("Your pi is now connected to thingsboard! (%u)\n", connect_status);

  return THINGSBOARD_OK;
}

THINGSBOARD_STATUS ThingsBoard_publish(unsigned char* topic, float value) {
  printf("Trying to publish data to your dashboard...\n");

  if (ThingsBoard_is_connected()) {
    unsigned char payload[64];
    snprintf(payload, sizeof(payload), "{\"%s\":%.2f}", topic, value);

    err_t ret = mqtt_publish(mqtt_client,
          "v1/devices/me/telemetry", &payload,
          strlen(payload), 0,
          0, NULL,
          LWIP_CONST_CAST(void*, &mqtt_client_info));

    printf("Value '%.2f' published to topic '%s'. (%u)\n", value, topic, ret);
    
    return THINGSBOARD_OK;
  } else {
    printf("Client not connected...\n");
    return THINGSBOARD_FAILED;
  }
}

THINGSBOARD_STATUS ThingsBoard_subscribe(void) {

}

THINGSBOARD_STATUS ThingsBoard_unsubscribe(void) {

}

THINGSBOARD_STATUS ThingsBoard_disconnect(void) {
  mqtt_disconnect(mqtt_client);
}

bool ThingsBoard_is_connected(void) {
    if (mqtt_client_is_connected(mqtt_client)) {
        return true;
    } else {
        return false;
    }
}
