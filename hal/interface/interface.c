#include "interface.h"

int8_t connect_to_interface(void) {
    /* Establish wifi connection
    SSID and Password are defined in secret.c */
    if (wifi_connect() != WIFI_CONNECTED)
        return -1;

    // Establish TCP/IP and MQTT connection
    printf("\n-------- Establishing ThingsBoard connection\n");
    ThingsBoard_connect();

    // Getting date&time from an official server
    // Then saving it into the RTC
    printf("\n-------- Getting current date & time\n");
    init_time();

    return 0;
}