#ifndef INTERFACE_H
#define INTERFACE_H

#include "wrap_WIFI.h"
#include "ThingsBoard.h"
#include "timing.h"

typedef enum _interface_status_t{
    INTERFACE_OK,
    INTERFACE_DISCONNECTED,
    INTERFACE_ERROR,
} interface_status_t;

interface_status_t connect_to_interface(void);
bool interface_is_connected(void);

#endif
