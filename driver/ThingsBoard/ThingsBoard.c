#include "ThingsBoard.h"

static mqtt_client_t* mqtt_client = NULL;

#define CONNECTION_TIMEOUT_MS     2000
#define CONNECTION_TEST_DELAY_MS  10
#define KEEP_ALIVE_TIMEOUT        65535
#define THINGSBOARD_HOSTNAME      "thingsboard.cloud"
#define MQTT_CONNECTION_RETRY     10

static const struct mqtt_connect_client_info_t mqtt_client_info =
{
  TB_CLIENT_ID, /* Client id */
  TB_USER, /* user, or access token in our case */
  TB_PASS, /* pass */
  KEEP_ALIVE_TIMEOUT,  /* keep alive */
  NULL, /* will_topic */
  NULL, /* will_msg */
  0,    /* will_qos */
  0     /* will_retain */
};


thingsboard_state_t ThingsBoard_connect(void) {
  printf("\n----- Establishing ThingsBoard connection -----\n");
  err_t connect_status;

  for (size_t i = 0; i < MQTT_CONNECTION_RETRY; i++) {
    feed_watchdog();
    connect_status = mqtt_connect(&mqtt_client, &mqtt_client_info, THINGSBOARD_HOSTNAME);

    if (connect_status == MQTT_CONNECT_ACCEPTED) {
      break;
    }
    vTaskDelay(1);
  }

  if (connect_status != MQTT_CONNECT_ACCEPTED) {
    return THINGSBOARD_FAILED;
  }

  for (size_t i = 0; i < CONNECTION_TIMEOUT_MS/CONNECTION_TEST_DELAY_MS; i++)
  {
    if (ThingsBoard_is_connected()) {
      printf("Connected after %d ms\n", i*CONNECTION_TEST_DELAY_MS);
      return THINGSBOARD_CONNECTED;
    }
    vTaskDelay(CONNECTION_TEST_DELAY_MS);
  }
  return THINGSBOARD_FAILED;
}

thingsboard_state_t ThingsBoard_publish(unsigned char* topic, float value) {
  if (!topic || !ThingsBoard_is_connected()) {
    return THINGSBOARD_DISCONNECTED;
  }

  // Get epoch time for exact timestamp
  uint64_t epoch_time;
  get_RTC_epoch_time(&epoch_time);

  // Creating the string that ThingsBoard will understand
  unsigned char payload[64];
  snprintf(payload, sizeof(payload),
      "{\"ts\":%llu, \"%s\":%.2f}", epoch_time, topic, value);

  err_t ret = mqtt_publish(mqtt_client,
        "v1/devices/me/telemetry", &payload,
        strlen(payload), 0,
        0, NULL,
        LWIP_CONST_CAST(void*, &mqtt_client_info));

  return ret == 0 ? THINGSBOARD_OK : THINGSBOARD_FAILED;
}

thingsboard_state_t ThingsBoard_subscribe(void) {

}

thingsboard_state_t ThingsBoard_unsubscribe(void) {

}

thingsboard_state_t ThingsBoard_disconnect(void) {
  mqtt_disconnect(mqtt_client);
}

bool ThingsBoard_is_connected(void) {
    return broker_is_connected();
}
