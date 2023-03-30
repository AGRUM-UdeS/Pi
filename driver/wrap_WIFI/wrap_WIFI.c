#include "wrap_WIFI.h"


#define WIFI_CONNECTION_MAX_RETRY 6
#define WIFI_CONNECTION_TIMEOUT_MS 10000

WIFI_STATUS wifi_connect(void) {
    printf("\n----- Connecting to the wifi -----\n");
    if (cyw43_arch_init_with_country(CYW43_COUNTRY_CANADA)) {
        printf("Failed to initialise\n");
        return WIFI_FAILED;
    }

    cyw43_arch_enable_sta_mode();

    for (size_t i = 0; i < WIFI_CONNECTION_MAX_RETRY; i++)
    {
        if (cyw43_arch_wifi_connect_timeout_ms(SSID, PASSWORD, CYW43_AUTH_WPA2_AES_PSK, WIFI_CONNECTION_TIMEOUT_MS)) {
            printf("Failed to connect. Retrying... (%d)\n", i);  
        } else {
            printf("Connected to the wifi.\n");
            return WIFI_CONNECTED;
        }
    }

    printf("Aborting wifi connection\n");
    return WIFI_FAILED;
}
