#ifndef CONTEXT_H
#define CONTEXT_H

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <stdio.h>
#include "energy_management.h"

typedef enum _interface_status_t {
    INTERFACE_CONNECTED,
    INTERFACE_DISCONNECTED,
    INTERFACE_ERROR
} interface_status_t;

typedef enum _irrigation_status_t {
    IRRIGATION_IDLE,
    IRRIGATION_MEASURING,
    IRRIGATION_PUMPING,
    IRRIGATION_IRRIGATING,
    IRRIGATION_ERROR
} irrigation_status_t;

// TODO: Add more states.
typedef struct _main_context {
    QueueHandle_t interface_queue_handle;
    interface_status_t interface_status;
    irrigation_status_t irrigation_status;
    energy_status_t energy_status;
} main_context_t;

extern main_context_t main_context;


#endif
