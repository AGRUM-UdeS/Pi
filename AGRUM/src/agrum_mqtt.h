#ifndef AGRUM_MQTT_H
#define AGRUM_MQTT_H

#include "lwip/apps/mqtt.h"
#include "lwip/dns.h"

void mqtt_connect(void);
void tb_pub(void);

#endif