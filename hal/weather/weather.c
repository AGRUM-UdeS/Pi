#include "weather.h"

#define HTTP_PORT   HTTP_DEFAULT_PORT
#define BUF_SIZE 2048
#define BUF_NB  16

static bool current_weather_is_received = false;
static bool weather_forecast_is_received = false;
static ip_addr_t weather_ipaddr;
static struct pbuf* weather_forecast_buf[BUF_NB];
static struct pbuf* current_weather_buf;
static uint16_t nb_packet = 0;

const char* WEATHER_REQUEST = 
"api.openweathermap.org";

void httpc_result_cb(void *arg, httpc_result_t httpc_result, 
                        u32_t rx_content_len, u32_t srv_res, err_t err) {
    // printf("transfer complete\n");
    // printf("local result=%d\n", httpc_result);
    // printf("http result=%d\n", srv_res);
}

err_t httpc_headers_cb(httpc_state_t *connection, void *arg, 
                            struct pbuf *hdr, u16_t hdr_len, u32_t content_len) {
    // printf("headers recieved\n");
    // printf("content length=%d\n", content_len);
    // printf("header length %d\n", hdr_len);

    return ERR_OK;
}

err_t forecast_cb(void *arg, struct altcp_pcb *conn, struct pbuf *p, err_t err) {
    // Save the weather buffer in a global variable
    weather_forecast_buf[nb_packet++] = p;
    if (nb_packet >= BUF_NB) {
        nb_packet = 0;
    }
    // Tell the application data has been received
    weather_forecast_is_received = true;
    
    return ERR_OK;
}

err_t current_weather_cb(void *arg, struct altcp_pcb *conn, struct pbuf *p, err_t err) {
    // Save the weather buffer in a global variable
    current_weather_buf = p;
    // Tell the application data has been received
    current_weather_is_received = true;
    
    return ERR_OK;
}

void weather_forecast_request(void) {
    httpc_connection_t settings;
    settings.result_fn = httpc_result_cb;
    settings.headers_done_fn = httpc_headers_cb;

    weather_forecast_is_received = false;

    httpc_get_file_dns(WEATHER_REQUEST, HTTP_PORT, MONTREAL_WEATHER_FORECAST, &settings, forecast_cb, NULL, NULL);
}

void weather_current_request(void) {
    printf("Requesting current weather data\n");
    httpc_connection_t settings;
    settings.result_fn = httpc_result_cb;
    settings.headers_done_fn = httpc_headers_cb;

    httpc_get_file_dns(WEATHER_REQUEST, HTTP_PORT, MONTREAL_WEATHER, &settings, current_weather_cb, NULL, NULL);
}

void print_weather_forecast(void) {
    static uint16_t i = 0;

    if ((!weather_forecast_is_received) || ((i >= nb_packet) && (nb_packet != 0))) {
        return;
    }

    struct pbuf* buf = weather_forecast_buf[i++];

    char* ptr = strstr(buf->payload, "\"temp\":");
    char num[] = {(char)(*(ptr+7)), (char)(*(ptr+8)), (char)(*(ptr+9)), 
                    (char)(*(ptr+10)), (char)(*(ptr+11)), (char)(*(ptr+12)), '\0'};
    printf("\n\nFirst temp is %s", num);
    printf(" => %f\n\n", atof(num));

    printf("%s", buf->payload);

    if (i >= BUF_NB) {
        i = 0;
    }
}

void print_current_weather(void) {
    static uint16_t i = 0;

    if (!current_weather_is_received) {
        return;
    }
    current_weather_is_received = false;

    struct pbuf* buf = current_weather_buf;

    // char* ptr = strstr(buf->payload, "\"temp\":");
    // char num[] = {(char)(*(ptr+7)), (char)(*(ptr+8)), (char)(*(ptr+9)), 
    //                 (char)(*(ptr+10)), (char)(*(ptr+11)), (char)(*(ptr+12)), '\0'};
    // printf("\n\nCurrent temp is %s", num);
    // printf(" => %f\n\n", atof(num));

    printf("%s", buf->payload);
}
