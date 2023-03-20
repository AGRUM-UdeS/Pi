#include "wifi.h"


#define WIFI_CONNECTION_MAX_RETRY 10
#define WIFI_CONNECTION_TIMEOUT_MS 10000

WIFI_STATUS wifi_connect(void) {
    if (cyw43_arch_init_with_country(CYW43_COUNTRY_CANADA)) {
        printf("Failed to initialise\n");
        return WIFI_FAILED;
    }

    cyw43_arch_enable_sta_mode();

    if (cyw43_arch_wifi_connect_timeout_ms(SSID, PASSWORD, CYW43_AUTH_WPA2_AES_PSK, WIFI_CONNECTION_TIMEOUT_MS)) {
        printf("Failed to connect. Retrying...\n");
        if (cyw43_arch_wifi_connect_timeout_ms(SSID, PASSWORD, CYW43_AUTH_WPA2_AES_PSK, WIFI_CONNECTION_TIMEOUT_MS)) {
            printf("Failed to connect. Aborting\n");
            return WIFI_FAILED;
        }
    }
    printf("Connected to the wifi.\n");
    return WIFI_CONNECTED;
}