#ifndef CONTEXT_H
#define CONTEXT_H

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <stdio.h>

#include "energy_management.h"
#include "irrigation.h"
#include "interface.h"

// TODO: Add more states.
typedef struct _main_context {
    QueueHandle_t interface_queue_handle;
    interface_status_t interface_status;
    irrigation_status_t irrigation_status;
    energy_status_t energy_status;
} main_context_t;

extern main_context_t main_context;


#endif
