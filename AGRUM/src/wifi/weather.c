#include "weather.h"
#include "secret.h"

#define HTTP_PORT   HTTP_DEFAULT_PORT
#define BUF_SIZE 2048

static bool weather_is_received = false;
static ip_addr_t weather_ipaddr;
static struct pbuf* weather_buf;

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
    // char myBuff[BUF_SIZE];
    // pbuf_copy_partial(hdr, myBuff, hdr->tot_len, 0);
    // printf("headers :\n");
    // printf("%s", myBuff);

    return ERR_OK;
}

err_t httpc_body_cb(void *arg, struct altcp_pcb *conn, struct pbuf *p, err_t err) {
    // Save the weather buffer in a global variable
    weather_buf = p;
    // Tell the application data has been received
    weather_is_received = true;
    
    return ERR_OK;
}


void init_weather(void) {
    httpc_connection_t settings;
    settings.result_fn = httpc_result_cb;
    settings.headers_done_fn = httpc_headers_cb;

    httpc_get_file_dns(WEATHER_REQUEST, HTTP_PORT, MONTREAL_WEATHER, &settings, httpc_body_cb, NULL, NULL);
    
    while (!weather_is_received) {
        tight_loop_contents();
    }
    struct pbuf* buf = weather_buf;

    printf("Packet received len:(%d), type:(%x), %x\n", buf->tot_len, buf->type_internal, buf->flags);
    printf("Weather data : %s\n", buf->payload);
    while (buf->next) {
        buf = buf->next;
        printf("%s\n", buf->payload);
    }
    printf("\nFINISH\n");

}
