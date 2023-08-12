/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "interface.h"
#include "PV_management.h"
#include "irrigation.h"
#include "energy_management.h"
#include "utils.h"
#include "test.h"

#include "weather.h"

/* Prototypes for the standard FreeRTOS callback/hook functions implemented
within this file. */
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );
void vApplicationTickHook( void );

#define HOUSEKEEPING_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1 )
#define STARTUP_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1 )

void startUp(void *pvParameters) {
    if (connect_to_interface() == INTERFACE_CONNECTED) {
        init_timer();
        interface_publish(PI_STATUS_TOPIC, PI_STATUS_CONNECTED);
    }
    // Init watchdog before wifi connection
    init_watchdog();

    feed_watchdog();

    init_peripherals();

    // Init everything irrigation related 
    init_irrigation();

    // Init everything solar panels related
    init_PV();

    // Init energy management
    enery_management();

    // Get weather data
    // printf("\n-------- Getting weather data\n");
    // weather_current_request();
    xTaskCreate( house_keeping,				/* The function that implements the task. */
            "house_keeping", 								/* The text name assigned to the task - for debug only as it is not used by the kernel. */
            configMINIMAL_STACK_SIZE, 			/* The size of the stack to allocate to the task. */
            NULL, 								/* The parameter passed to the task - not used in this case. */
            HOUSEKEEPING_TASK_PRIORITY, 	/* The priority assigned to the task. */
            NULL );								/* The task handle is not required, so NULL is passed. */

    while(1) {
        interface_status_t status_interface = interface_sm();
        irrigation_status_t status_irrigation = irrigation_sm();
        energy_status_t status_energy = enery_management();
        send_system_status(status_interface, status_irrigation, status_energy);
        vTaskDelay(50);
    }
}

int main() {
    // Init RP2040 peripherals
    stdio_init_all();

    // Delay to let the developer open Putty
    usb_delay(5);

    xTaskCreate( startUp,				/* The function that implements the task. */
            "startUp", 								/* The text name assigned to the task - for debug only as it is not used by the kernel. */
            configMINIMAL_STACK_SIZE, 			/* The size of the stack to allocate to the task. */
            NULL, 								/* The parameter passed to the task - not used in this case. */
            STARTUP_TASK_PRIORITY, 	/* The priority assigned to the task. */
            NULL );

    /* Start the tasks and timer running. */
	vTaskStartScheduler();
    // while (true) {
    //     house_keeping();
    //     interface_status_t status_interface = interface_sm();
    //     irrigation_status_t status_irrigation = irrigation_sm();
    //     energy_status_t status_energy = enery_management();
    //     send_system_status(status_interface, status_irrigation, status_energy);
    //     //develop_test();
    //     sleep_ms(50);

    // }
    return 0;
}

void vApplicationMallocFailedHook( void )
{
    /* Called if a call to pvPortMalloc() fails because there is insufficient
    free memory available in the FreeRTOS heap.  pvPortMalloc() is called
    internally by FreeRTOS API functions that create tasks, queues, software
    timers, and semaphores.  The size of the FreeRTOS heap is set by the
    configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */

    /* Force an assert. */
    configASSERT( ( volatile void * ) NULL );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */

    /* Force an assert. */
    configASSERT( ( volatile void * ) NULL );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
    volatile size_t xFreeHeapSpace;

    /* This is just a trivial example of an idle hook.  It is called on each
    cycle of the idle task.  It must *NOT* attempt to block.  In this case the
    idle task just queries the amount of FreeRTOS heap that remains.  See the
    memory management section on the http://www.FreeRTOS.org web site for memory
    management options.  If there is a lot of heap memory free then the
    configTOTAL_HEAP_SIZE value in FreeRTOSConfig.h can be reduced to free up
    RAM. */
    xFreeHeapSpace = xPortGetFreeHeapSize();

    /* Remove compiler warning about xFreeHeapSpace being set but never used. */
    ( void ) xFreeHeapSpace;
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
#if mainCREATE_SIMPLE_BLINKY_DEMO_ONLY == 0
    {
        /* The full demo includes a software timer demo/test that requires
        prodding periodically from the tick interrupt. */
        #if (mainENABLE_TIMER_DEMO == 1)
        vTimerPeriodicISRTests();
        #endif

        /* Call the periodic queue overwrite from ISR demo. */
        #if (mainENABLE_QUEUE_OVERWRITE == 1)
        vQueueOverwritePeriodicISRDemo();
        #endif

        /* Call the periodic event group from ISR demo. */
        #if (mainENABLE_EVENT_GROUP == 1)
        vPeriodicEventGroupsProcessing();
        #endif

        /* Call the code that uses a mutex from an ISR. */
        #if (mainENABLE_INTERRUPT_SEMAPHORE == 1)
        vInterruptSemaphorePeriodicTest();
        #endif

        /* Call the code that 'gives' a task notification from an ISR. */
        #if (mainENABLE_TASK_NOTIFY == 1)
        xNotifyTaskFromISR();
        #endif
    }
#endif
}