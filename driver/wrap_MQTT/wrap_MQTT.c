#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "wrap_MQTT.h"
#include "lwipopts.h"

#define MQTT_PORT       LWIP_IANA_PORT_MQTT

static ip_addr_t mqtt_ipaddr;

static bool host_name_is_resolved = false;
static bool is_thingsboard_connected = false;

void dns_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg)
{
    if (ipaddr == NULL) {
        // Failed to resolve hostname
        printf("Error resolving hostname: %s\n", name);
    } else {
        // Hostname was resolved successfully
        // printf("IP address for %s: %d.%d.%d.%d\n", name,
        //        ip4_addr1(ipaddr), ip4_addr2(ipaddr),
        //        ip4_addr3(ipaddr), ip4_addr4(ipaddr));
        mqtt_ipaddr = *ipaddr;
        host_name_is_resolved = true;
    }
}

static void resolve_hostname(const char *hostname)
{
    ip_addr_t ip_addr, dns_server;
    IP4_ADDR(&dns_server, 8, 8, 8, 8);

    dns_setserver(0, &dns_server);
    cyw43_arch_lwip_begin();
    err_t err = dns_gethostbyname(hostname, &ip_addr, dns_callback, NULL);
    cyw43_arch_lwip_end();
    if (err == ERR_OK) {
        // IP address was resolved successfully
        printf("IP address: %d.%d.%d.%d\n",
               ip4_addr1(&ip_addr), ip4_addr2(&ip_addr),
               ip4_addr3(&ip_addr), ip4_addr4(&ip_addr));
        mqtt_ipaddr = ip_addr;
        host_name_is_resolved = true;
    }
}

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

  LWIP_PLATFORM_DIAG(("MQTT client \"%s\" connection status: %d\n", client_info->client_id, (int)status));

  if (mqtt_client_is_connected(client)) {
    is_thingsboard_connected = true;
    // printf("Your pi is now connected to thingsboard!(%u)\n", (int)status);
  } else {
    is_thingsboard_connected = false;
    printf("Your pi has disconnected to thingsboard...(%u)\n", (int)status);
  }

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

err_t mqtt_connect(mqtt_client_t** mqtt_client, const struct mqtt_connect_client_info_t* client_info, const char *hostname) {
  *mqtt_client = mqtt_client_new();

  mqtt_set_inpub_callback(*mqtt_client,
        mqtt_incoming_publish_cb,
        mqtt_incoming_data_cb,
        LWIP_CONST_CAST(void*, client_info));

  resolve_hostname(hostname);

  while (!host_name_is_resolved) {
    tight_loop_contents();
  }

  err_t ret = mqtt_client_connect(*mqtt_client,
        &mqtt_ipaddr, MQTT_PORT,
        mqtt_connection_cb, LWIP_CONST_CAST(void*, client_info),
        client_info);

  return ret;
}

bool thingsboard_is_connected(void) {
  return is_thingsboard_connected;
}
