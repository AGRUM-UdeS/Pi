#include <stdio.h>
#include <stdlib.h>

#include "agrum_mqtt.h"

/** thingsboard.cloud = 44.193.166.37 */
#define LWIP_MQTT_IPADDR_INIT = IPADDR4_INIT_BYTES(44, 193, 166, 37);
#define MQTT_PORT       LWIP_IANA_PORT_MQTT

static ip_addr_t mqtt_ip LWIP_MQTT_IPADDR_INIT;
static mqtt_client_t* mqtt_client;


static const struct mqtt_connect_client_info_t mqtt_client_info =
{
  "e45ded90-982b-11ed-a183-5d40b1136f31", /* Client id */
  "TLvbrgj6h79B31uhKF6Z", /* user, or access token in our case */
  "CitronPresse65", /* pass */
  100,  /* keep alive */
  NULL, /* will_topic */
  NULL, /* will_msg */
  0,    /* will_qos */
  0     /* will_retain */
#if LWIP_ALTCP && LWIP_ALTCP_TLS
  , NULL
#endif
};

static void
mqtt_request_cb(void *arg, err_t err)
{
  const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;

  LWIP_PLATFORM_DIAG(("MQTT client \"%s\" request cb: err %d\n", client_info->client_id, (int)err));
}

static void mqtt_connection_cb(mqtt_client_t *client,
            void *arg, mqtt_connection_status_t status)
{
  const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;
  LWIP_UNUSED_ARG(client);

  LWIP_PLATFORM_DIAG(("MQTT client \"%s\" connection cb: status %d\n", client_info->client_id, (int)status));

  if (status == MQTT_CONNECT_ACCEPTED) {
    mqtt_sub_unsub(client,
            "topic_qos1", 1,
            mqtt_request_cb, LWIP_CONST_CAST(void*, client_info),
            1);
    mqtt_sub_unsub(client,
            "topic_qos0", 0,
            mqtt_request_cb, LWIP_CONST_CAST(void*, client_info),
            1);
  }
}

static void
mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
  const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;
  LWIP_UNUSED_ARG(data);

  LWIP_PLATFORM_DIAG(("MQTT client \"%s\" data cb: len %d, flags %d\n", client_info->client_id,
          (int)len, (int)flags));
}

static void
mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
  const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;

  LWIP_PLATFORM_DIAG(("MQTT client \"%s\" publish cb: topic %s, len %d\n", client_info->client_id,
          topic, (int)tot_len));
}

void mqtt_connect(void) {
    printf("Trying to connect to thingsboard...\n");

    mqtt_client = mqtt_client_new();

    printf("Setting callback fct..\n");

    mqtt_set_inpub_callback(mqtt_client,
        mqtt_incoming_publish_cb,
        mqtt_incoming_data_cb,
        LWIP_CONST_CAST(void*, &mqtt_client_info));

    printf("Client connecting..\n");

    err_t ret = mqtt_client_connect(mqtt_client,
          &mqtt_ip, MQTT_PORT,
          mqtt_connection_cb, LWIP_CONST_CAST(void*, &mqtt_client_info),
          &mqtt_client_info);

    printf("Client connected?..(%u)\n", ret);

}

void tb_pub(void) {
    printf("Trying to publish data to thingsboard...\n");

    const char* pld = "30";
    u16_t sz = sizeof(*pld);

    err_t ret = mqtt_publish(mqtt_client,
                    "eau", pld,
                    sz, 0,
                    0, NULL,
                    LWIP_CONST_CAST(void*, &mqtt_client_info));

    printf("Client connected?..(%u)\n", ret);

}

