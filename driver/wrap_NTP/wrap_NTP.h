#ifndef NTP_H
#define NTP_H

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/util/datetime.h"

#include "lwip/dns.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"

#include "wrap_RTC.h"

typedef enum _ntp_status_t{
    NTP_OK,
    NTP_FAILED
} ntp_status_t;

typedef struct NTP_T_ {
    ip_addr_t ntp_server_address;
    bool dns_request_sent;
    struct udp_pcb *ntp_pcb;
    absolute_time_t ntp_test_time;
    alarm_id_t ntp_resend_alarm;
} NTP_T;

bool ntp_time_received(void);
ntp_status_t get_time_ntp(datetime_t* time);
struct tm get_utc(void);


#endif