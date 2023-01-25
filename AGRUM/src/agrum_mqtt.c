#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "agrum_mqtt.h"

#define MQTT_PORT       LWIP_IANA_PORT_MQTT
#define THINGSBOARD_HOSTNAME "thingsboard.cloud"
#define KEEP_ALIVE_TIMEOUT 3600

static mqtt_client_t* mqtt_client;
static ip_addr_t tb_ipaddr;
static bool host_name_is_resolved = false;

void dns_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg)
{
    if (ipaddr == NULL) {
        // Failed to resolve hostname
        printf("Error resolving hostname: %s\n", name);
    } else {
        // Hostname was resolved successfully
        printf("IP address for %s: %d.%d.%d.%d\n", name,
               ip4_addr1(ipaddr), ip4_addr2(ipaddr),
               ip4_addr3(ipaddr), ip4_addr4(ipaddr));
        tb_ipaddr = *ipaddr;
        host_name_is_resolved = true;
    }
}

static void get_ip_addr(const char *hostname)
{
    ip_addr_t ip_addr, dns_server;
    IP4_ADDR(&dns_server, 8, 8, 8, 8);

    dns_setserver(0, &dns_server);

    err_t err = dns_gethostbyname(hostname, &ip_addr, dns_callback, NULL);
    if (err == ERR_OK) {
        // IP address was resolved successfully
        printf("IP address: %d.%d.%d.%d\n",
               ip4_addr1(&ip_addr), ip4_addr2(&ip_addr),
               ip4_addr3(&ip_addr), ip4_addr4(&ip_addr));
        tb_ipaddr = ip_addr;
        host_name_is_resolved = true;
    } else if (err == ERR_INPROGRESS) {
        printf("Resolving '%s' hostname...\n", hostname);
    }
}

// {clientId:"e45ded90-982b-11ed-a183-5d40b1136f31",userName:"TLvbrgj6h79B31uhKF6Z",password:"CitronPresse65"} //
static const struct mqtt_connect_client_info_t mqtt_client_info =
{
  //"RaspberryPiPicoW", /* Client id */
  "e45ded90-982b-11ed-a183-5d40b1136f31", /* Client id */
  "TLvbrgj6h79B31uhKF6Z", /* user, or access token in our case */
  "CitronPresse65", /* pass */
  KEEP_ALIVE_TIMEOUT,  /* keep alive */
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

    mqtt_set_inpub_callback(mqtt_client,
        mqtt_incoming_publish_cb,
        mqtt_incoming_data_cb,
        LWIP_CONST_CAST(void*, &mqtt_client_info));

    get_ip_addr(THINGSBOARD_HOSTNAME);

    printf("Wating for hostname to be resolved...\n");

    while (!host_name_is_resolved){}

    err_t ret = mqtt_client_connect(mqtt_client,
          &tb_ipaddr, MQTT_PORT,
          mqtt_connection_cb, LWIP_CONST_CAST(void*, &mqtt_client_info),
          &mqtt_client_info);

    printf("Client connected! (%u)\n", ret);
}

void tb_pub(void) {
    printf("Trying to publish data to thingsboard...\n");

    if (mqtt_client_is_connected(mqtt_client)) {
        u16_t pld = 30;
        u16_t sz = sizeof(pld);

        err_t ret = mqtt_publish(mqtt_client,
                        "eau", &pld,
                        sz, 0,
                        0, NULL,
                        LWIP_CONST_CAST(void*, &mqtt_client_info));

        printf("Message published! (%u)\n", ret);
    } else {
        printf("Client not connected...\n");
    }
}

