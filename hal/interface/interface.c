#include "interface.h"

uint8_t connect_to_interface(void) {
    /* Establish wifi connection
    SSID and Password are defined in secret.c */
    printf("---------- Connecting to the wifi\n\n");
    wifi_connect();

    // Establish TCP/IP and MQTT connection
    // printf("\n-------- Establishing ThingsBoard connection\n");
    // ThingsBoard_connect();

    // // Getting date&time from an official server
    // // Then saving it into the RTC
    // printf("\n-------- Getting current date & time\n");
    // init_RTC();

    return 0;
}