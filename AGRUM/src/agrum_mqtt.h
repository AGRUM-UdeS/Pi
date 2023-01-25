#ifndef AGRUM_MQTT_H
#define AGRUM_MQTT_H

#include "lwip/apps/mqtt.h"
#include "lwip/dns.h"

void thinsboard_connect(void);
void thinsboard_pub(void);
void thinsboard_disconnect(void);


#endif