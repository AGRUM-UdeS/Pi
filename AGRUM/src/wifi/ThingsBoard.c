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


thingsboard_state_t ThingsBoard_connect(void) {
  err_t connect_status = mqtt_connect(&mqtt_client, &mqtt_client_info, THINGSBOARD_HOSTNAME);

  while (!(ThingsBoard_is_connected())) {
    tight_loop_contents();
  }
  
  return (ThingsBoard_is_connected() ? THINGSBOARD_CONNECTED : THINGSBOARD_DISCONNECTED);
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

    printf("Value '%.2f' published to topic '%s'. (%u)\n", value, topic, ret);
    
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

thingsboard_state_t thingsboard_sm(void) {
    // thingsboard_state represente the state of the state machine
    static thingsboard_state_t thingsboard_state = THINGSBOARD_IDLE;

    // thingsboard_status represent the status of
    // the connection to thingsboard
    static thingsboard_status_t thingsboard_status;

    switch (thingsboard_state) {
    case THINGSBOARD_IDLE:
      if (!ThingsBoard_is_connected()) {
        thingsboard_state = THINGSBOARD_CONNECTING;
      }

        break;

    case THINGSBOARD_CONNECTING:
      ThingsBoard_connect();
      thingsboard_state = THINGSBOARD_IDLE;
        break;

    case THINGSBOARD_SEND_DATA:

        break;

    case THINGSBOARD_READ_DATA:

        break;

    case THINGSBOARD_ERROR:

        break;
    }


    return thingsboard_state;
}
