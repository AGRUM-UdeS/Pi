#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdio.h>
#include "interface.h"
#include "irrigation.h"
#include "energy_management.h"

// TODO: Add more states.
typedef struct _main_context {
    interface_status_t interface_status;
    irrigation_status_t irrigation_status;
    energy_status_t energy_status;
} main_context_t;

extern main_context_t main_context;


#endif
