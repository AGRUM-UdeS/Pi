#include "ThingsBoard.h"

static mqtt_client_t* mqtt_client;

#define CONNECTION_TIMEOUT_MS     200
#define CONNECTION_TEST_DELAY_MS  5
#define KEEP_ALIVE_TIMEOUT        3600
#define THINGSBOARD_HOSTNAME      "thingsboard.cloud"

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


thingsboard_state_t ThingsBoard_connect(void) {
  printf("\n----- Establishing ThingsBoard connection -----\n");
  err_t connect_status = mqtt_connect(&mqtt_client, &mqtt_client_info, THINGSBOARD_HOSTNAME);

  for (size_t i = 0; i < CONNECTION_TIMEOUT_MS/CONNECTION_TEST_DELAY_MS; i++)
  {
    if (ThingsBoard_is_connected()) {
      printf("Connected after %d ms\n", i*CONNECTION_TEST_DELAY_MS);
      return THINGSBOARD_CONNECTED;
    }
    sleep_ms(CONNECTION_TEST_DELAY_MS);
  }
  return THINGSBOARD_FAILED;
}

thingsboard_state_t ThingsBoard_publish(unsigned char* topic, float value) {
  if (ThingsBoard_is_connected()) {
    // Creating the string that ThingsBoard will understand
    unsigned char payload[64];
    snprintf(payload, sizeof(payload), "{\"%s\":%.2f}", topic, value);

    err_t ret = mqtt_publish(mqtt_client,
          "v1/devices/me/telemetry", &payload,
          strlen(payload), 0,
          0, NULL,
          LWIP_CONST_CAST(void*, &mqtt_client_info));

    printf("Value '%.2f' published to topic '%s'. (%d)\n", value, topic, ret);
    
    return ret == 0 ? THINGSBOARD_OK : THINGSBOARD_FAILED;
  } else {
    printf("Client not connected...\n");
    return THINGSBOARD_DISCONNECTED;
  }
}

thingsboard_state_t ThingsBoard_subscribe(void) {

}

thingsboard_state_t ThingsBoard_unsubscribe(void) {

}

thingsboard_state_t ThingsBoard_disconnect(void) {
  mqtt_disconnect(mqtt_client);
}

bool ThingsBoard_is_connected(void) {
    if (mqtt_client_is_connected(mqtt_client)) {
        return true;
    } else {
        return false;
    }
}
