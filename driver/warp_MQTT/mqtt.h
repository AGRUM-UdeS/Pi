#ifndef MQTT_H
#define MQTT_H

#include "lwip/apps/mqtt.h"
#include "lwip/dns.h"
#include "pico/cyw43_arch.h"

err_t mqtt_connect(mqtt_client_t** mqtt_client, const struct mqtt_connect_client_info_t* client_info, const char *hostname);

#endif