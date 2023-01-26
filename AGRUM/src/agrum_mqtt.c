#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "agrum_mqtt.h"
#include "lwipopts.h"

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

static void resolve_hostname(const char *hostname)
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
    // Do other things like sub to a topic
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

void thinsboard_connect(void) {
  printf("Trying to connect to thingsboard...\n");

  mqtt_client = mqtt_client_new();

  mqtt_set_inpub_callback(mqtt_client,
        mqtt_incoming_publish_cb,
        mqtt_incoming_data_cb,
        LWIP_CONST_CAST(void*, &mqtt_client_info));

  resolve_hostname(THINGSBOARD_HOSTNAME);

  printf("Wating for hostname to be resolved...\n");

  while (!host_name_is_resolved){}

  err_t ret = mqtt_client_connect(mqtt_client,
        &tb_ipaddr, MQTT_PORT,
        mqtt_connection_cb, LWIP_CONST_CAST(void*, &mqtt_client_info),
        &mqtt_client_info);

  printf("Your pi is now connected to thingsboard! (%u)\n", ret);
}

void thinsboard_pub(void) {
  printf("Trying to publish data to your dashboard...\n");

  if (mqtt_client_is_connected(mqtt_client)) {
    unsigned char payload[64];
    unsigned char topic[] = "temperature";
    unsigned char value_string[8];
    static float value = -5.1;

    snprintf(value_string, sizeof(value_string), "%.2f", value++);
    snprintf(payload, sizeof(payload), "{\"%s\":%s}", topic, value_string);

    err_t ret = mqtt_publish(mqtt_client,
          "v1/devices/me/telemetry", &payload,
          strlen(payload), 0,
          0, NULL,
          LWIP_CONST_CAST(void*, &mqtt_client_info));

    printf("Value '%s' published to topic '%s'. (%u)\n", value_string, topic, ret);
  } else {
    printf("Client not connected...\n");
  }
}

void thinsboard_disconnect(void) 
{
  mqtt_disconnect(mqtt_client);
}

