#include "weather.h"

#define HTTP_PORT   HTTP_DEFAULT_PORT
#define BUF_SIZE    2048
#define BUF_NB      4

#define NB_CHAR     3

static bool weather_is_received = false;
static bool new_weather_request = false;
static bool weather_cb_flag = false;

static char myBuff[BUF_NB][BUF_SIZE] = {0};
static uint8_t myBuff_index = 0;

const char* WEATHER_REQUEST = 
"api.open-meteo.com";

const char* MONTREAL_WEATHER = 
"/v1/forecast?latitude=45.53&longitude=-73.52&hourly=cloudcover&daily=sunrise,sunset,precipitation_sum,windgusts_10m_max&timeformat=unixtime&forecast_days=3&timezone=America%2FNew_York";

const char* NEW_LINE = "\n";
const char* END_OF_MSG = "}}";
const char* COMMA = ",";

const char* CLOUDCOVER_STR = "cloudcover";
const char* SUNRISE_STR = "sunrise";
const char* SUNSET_STR = "sunset";
const char* PRECIPITATION_STR = "precipitation_sum";
const char* WINDGUST_STR = "windgusts_10m_max";

#define WEATHER_ALARM_HOUR      (6)
#define WEATHER_ALARM_MINUTE    (30)

static weather_handle_t* weather_handle_ptr;

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

static void cat_buf(char** dest, char source[][BUF_SIZE], uint8_t buf_nb) {
    // Get the size of the string
    size_t len = 0;
    for (size_t i = 0; i <= buf_nb; i++) {
        len += strlen(source[i]);
    }

    char *temp = malloc(len + 1);
    if (temp == NULL)
        return;

    // Concatenate into one string
    for (size_t i = 0; i <= buf_nb; i++){
        strcat(temp, source[i]);
    }

    // Delete garbage before and after payload
    // This is custom code for the weather API used
    strtok(temp, NEW_LINE);
    char *test = strtok(NULL, NEW_LINE);
    free(temp);

    // Save string into dest
    *dest = malloc(strlen(test) + 1);
    if (*dest != NULL)
        strcpy(*dest, test);
}

static bool meteo_parse(char* str, weather_forecast_t* weather_forecast_in)
{
    char* buf = malloc(strlen(str));
    strcpy(buf, str);

    // Get beginning of cloudcover
    char* token = strstr(buf, CLOUDCOVER_STR);
    token = strstr(token+1, CLOUDCOVER_STR);
    // start now point to the beginning of the data
    token = strtok(token+strlen(CLOUDCOVER_STR)+NB_CHAR, COMMA);
    if (token != NULL) {
        // printf("%s :\n", CLOUDCOVER_STR);
        for (size_t i = 0; i < FORECAST_HOURS; i++) {
            weather_forecast_in->cloudcover_hourly[i] = atoi(token);
            token = strtok(NULL, COMMA);
            // printf("%d\n", weather_forecast_in->cloudcover_hourly[i]);
        }
    } else {
        printf("%s notfound..\n", CLOUDCOVER_STR);
    }

    strcpy(buf, str);
    // Get beginning of sunrise
    token = strstr(buf, SUNRISE_STR);
    token = strstr(token+1, SUNRISE_STR);
    token = strtok(token+strlen(SUNRISE_STR)+NB_CHAR, COMMA);
    if (token != NULL) {
        // printf("%s :\n", SUNRISE_STR);
        for (size_t i = 0; i < FORECAST_DAYS; i++) {
            weather_forecast_in->sunrise[i] = atoll(token);
            token = strtok(NULL, COMMA);
            // printf("%llu\n", weather_forecast_in->sunrise[i]);
        }
    } else {
        printf("%s notfound...\n", SUNRISE_STR);
    }

    strcpy(buf, str);
    // Get beginning of sunset
    token = strstr(buf, SUNSET_STR);
    token = strstr(token+1, SUNSET_STR);
    token = strtok(token+strlen(SUNSET_STR)+NB_CHAR, COMMA);
    if (token != NULL) {
        // printf("%s :\n", SUNSET_STR);
        for (size_t i = 0; i < FORECAST_DAYS; i++) {
            weather_forecast_in->sunset[i] = atoll(token);
            token = strtok(NULL, COMMA);
            // printf("%llu\n", weather_forecast_in->sunset[i]);
        }
    } else {
        printf("%s notfound...\n", SUNSET_STR);
    }

    strcpy(buf, str);
    // Get beginning of precipitation
    token = strstr(buf, PRECIPITATION_STR);
    token = strstr(token+1, PRECIPITATION_STR);
    token = strtok(token+strlen(PRECIPITATION_STR)+NB_CHAR, COMMA);
    if (token != NULL) {
        // printf("%s :\n", PRECIPITATION_STR);
        for (size_t i = 0; i < FORECAST_DAYS; i++) {
            weather_forecast_in->precipitation_daily[i] = atof(token);
            token = strtok(NULL, COMMA);
            // printf("%.2f\n", weather_forecast_in->precipitation_daily[i]);
        }
    } else {
        printf("%s notfound...\n", PRECIPITATION_STR);
    }

    strcpy(buf, str);
    // Get beginning of windgust max
    token = strstr(buf, WINDGUST_STR);
    token = strstr(token+1, WINDGUST_STR);
    token = strtok(token+strlen(WINDGUST_STR)+NB_CHAR, COMMA);
    if (token != NULL) {
        // printf("%s :\n", WINDGUST_STR);
        for (size_t i = 0; i < FORECAST_DAYS; i++) {
            weather_forecast_in->max_windgust_daily[i] = atof(token);
            token = strtok(NULL, COMMA);
            // printf("%.2f\n", weather_forecast_in->max_windgust_daily[i]);
        }
    } else {
        printf("%s notfound...\n", WINDGUST_STR);
    }
    return true;
}

static weather_forecast_t weather_current_request(void) {
    httpc_connection_t settings;
    settings.result_fn = httpc_result_cb;
    settings.headers_done_fn = httpc_headers_cb;

    // Make a new request
    new_weather_request = true;
    httpc_get_file_dns(WEATHER_REQUEST, HTTP_PORT, MONTREAL_WEATHER, &settings, httpc_body_cb, NULL, NULL);

    // Wait for all the msg to be received
    while (!weather_is_received) {
        // If failing, watchdog will reboot
        tight_loop_contents();
    }
    weather_is_received = false;

    // Concatenate the buffers into one string
    char *meteo_str;
    cat_buf(&meteo_str, myBuff, myBuff_index);

    // printf("Finale string : %s\n", meteo_str);
    weather_forecast_t weather_forecast;
    if (meteo_parse(meteo_str, &weather_forecast)) {
        printf("Weather data received!\n");
    } else {
        printf("Failed to receive weather data\n");
    }

    // DONT FORGET TO FREE meteo_str
    free(meteo_str);
    return weather_forecast;
}

static void weather_alarm_callback(void)
{
    printf("WEATHER REQUEST!\n");
    weather_cb_flag = true;
}

void weather_init(weather_handle_t* weather_handle)
{
    // Get intial weather
    printf("WEATHER RECEIVED1!\n");
    weather_handle->weather_forecast = weather_current_request();
    // Tell the app forecast was received
    weather_handle->is_received = true;

    // Alarm once a minute
    datetime_t weather_alarm = {
        .year  = -1,
        .month = -1,
        .day   = -1,
        .dotw  = -1,
        .hour  = WEATHER_ALARM_HOUR,
        .min   = WEATHER_ALARM_MINUTE,
        .sec   = 0,
    };

    rtc_set_alarm(&weather_alarm, &weather_alarm_callback);
}

weather_status_t weather_task(weather_handle_t* weather_handle)
{
    if (!weather_cb_flag) {
        return WEATHER_ERROR;
    }

    weather_cb_flag = false;
    weather_handle->weather_forecast = weather_current_request();
    weather_handle->is_received = true;
    return WEATHER_OK;
}

void weather_printf(weather_handle_t* weather_handle)
{
    printf("\tPrecipitation\tWindgust\tSunrise\tSunset\n");
    for (size_t i = 0; i < FORECAST_DAYS; i++) {
        printf("Day %d:\t%.2f,\t%.2f,\t%llu,\t%llu\n",
                i, weather_handle->weather_forecast.precipitation_daily[i], weather_handle->weather_forecast.max_windgust_daily[i],
                weather_handle->weather_forecast.sunrise[i], weather_handle->weather_forecast.sunset[i]);
    }

    printf("\tcloudcover\n");
    for (size_t i = 0; i < FORECAST_HOURS; i++) {
        printf("Hour %2d:\t%d\n",
                i, weather_handle->weather_forecast.cloudcover_hourly[i]);
    }
    
}
