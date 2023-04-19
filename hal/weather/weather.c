#include "weather.h"

#define HTTP_PORT   HTTP_DEFAULT_PORT
#define BUF_SIZE 2048
#define BUF_NB   4

static bool weather_is_received = false;
static bool new_weather_request = false;

static char myBuff[BUF_NB][BUF_SIZE] = {0};
static uint8_t myBuff_index = 0;

const char* WEATHER_REQUEST = 
"api.open-meteo.com";

const char* MONTREAL_WEATHER = 
"/v1/forecast?latitude=45.53&longitude=-73.52&hourly=rain,cloudcover,windspeed_80m,direct_radiation_instant&timeformat=unixtime&forecast_days=3&timezone=America%2FNew_York";

const char* END_OF_MSG = "]}}";

void httpc_result_cb(void *arg, httpc_result_t httpc_result, 
                        u32_t rx_content_len, u32_t srv_res, err_t err) {
    // printf("transfer complete\n");
    // printf("local result=%d\n", httpc_result);
    // printf("http result=%d\n", srv_res);
}

err_t httpc_headers_cb(httpc_state_t *connection, void *arg, 
                            struct pbuf *hdr, u16_t hdr_len, u32_t content_len) {
    printf("headers recieved\n");
    printf("content length=%d\n", content_len);
    printf("header length %d\n", hdr_len);
    // char myBuff[BUF_SIZE];
    // pbuf_copy_partial(hdr, myBuff, hdr->tot_len, 0);
    // printf("headers :\n");
    // printf("%s", myBuff);

    return ERR_OK;
}

err_t httpc_body_cb(void *arg, struct altcp_pcb *conn, struct pbuf *p, err_t err) {
    if (new_weather_request != true) {
        return ERR_OK;
    }

    // Save the weather buffer in a global variable
    static uint8_t i = 0;
    pbuf_copy_partial(p, myBuff[i], p->tot_len, 0);

    char *ptr = NULL;
    ptr = strstr(myBuff[i], END_OF_MSG);
    if (ptr != NULL) {
        // Tell the application data has been received
        weather_is_received = true;
        new_weather_request = false;
        myBuff_index = i;
        i = 0;
    } else {
        i++;
    }
    return ERR_OK;
}

void weather_current_request(void) {
    httpc_connection_t settings;
    settings.result_fn = httpc_result_cb;
    settings.headers_done_fn = httpc_headers_cb;

    new_weather_request = true;
    httpc_get_file_dns(WEATHER_REQUEST, HTTP_PORT, MONTREAL_WEATHER, &settings, httpc_body_cb, NULL, NULL);
    
    while (!weather_is_received) {
        tight_loop_contents();
    }
    weather_is_received = false;

    printf("Body (%d) :\n", myBuff_index);
    for (size_t i = 0; i <= myBuff_index; i++)
    {
        printf("%s", myBuff[i]);
    }
    printf("\n");
}

void print_current_weather(void) {
    // static uint16_t i = 0;

    // if (!weather_is_received) {
    //     return;
    // }
    // weather_is_received = false;

    // struct pbuf* buf = weather_buf;

    // // char* ptr = strstr(buf->payload, "\"temp\":");
    // // char num[] = {(char)(*(ptr+7)), (char)(*(ptr+8)), (char)(*(ptr+9)), 
    // //                 (char)(*(ptr+10)), (char)(*(ptr+11)), (char)(*(ptr+12)), '\0'};
    // // printf("\n\nCurrent temp is %s", num);
    // // printf(" => %f\n\n", atof(num));

    // printf("%s", buf->payload);
}
