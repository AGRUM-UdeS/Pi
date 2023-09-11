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
#include "context.h"
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

#define PV_MANAGEMENT_TASK_PRIORITY     ( tskIDLE_PRIORITY + 3 )
#define IRRIGATION_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )
#define HOUSEKEEPING_TASK_PRIORITY      ( tskIDLE_PRIORITY + 2 )
#define STARTUP_TASK_PRIORITY           ( tskIDLE_PRIORITY + 3 )
#define INTERFACE_TASK_PRIORITY         ( tskIDLE_PRIORITY + 5 )
#define WEATHER_TASK_PRIORITY           ( tskIDLE_PRIORITY + 4 )

main_context_t main_context;

void startUp(void *pvParameters) {
    if (connect_to_interface() == INTERFACE_CONNECTED) {
        init_timer();
    }
    init_peripherals();

    // Init energy management
    // enery_management();

    // Get weather data
    // printf("\n-------- Getting weather data\n");
    // weather_current_request();

    // xTaskCreate( irrigation_management,
    //         "irrigation_management",
    //         configMINIMAL_STACK_SIZE,
    //         &main_context,
    //         IRRIGATION_TASK_PRIORITY,
    //         NULL );

    xTaskCreate( interface,
            "interface",
            configMINIMAL_STACK_SIZE,
            &main_context,
            INTERFACE_TASK_PRIORITY,
            NULL );

    xTaskCreate( house_keeping,
            "house_keeping",
            configMINIMAL_STACK_SIZE,
            &main_context,
            HOUSEKEEPING_TASK_PRIORITY,
            NULL );

    xTaskCreate( PV_management,
            "PV_management",
            configMINIMAL_STACK_SIZE,
            &main_context,
            PV_MANAGEMENT_TASK_PRIORITY,
            NULL );

        // xTaskCreate( weather_task,
        //     "weather_task",
        //     configMINIMAL_STACK_SIZE,
        //     &main_context,
        //     WEATHER_TASK_PRIORITY,
        //     NULL );

    while(1) {
        // irrigation_status_t status_irrigation = irrigation_sm();
        // energy_status_t status_energy = enery_management();
        //send_system_status(status_interface, status_irrigation, status_energy);
        //develop_test();
        vTaskDelay(portMAX_DELAY);
    }
}

int main() {
    // Init RP2040 peripherals
    stdio_init_all();

    // Delay to let the developer open Putty
    usb_delay(5);

    xTaskCreate( startUp,               /* The function that implements the task. */
            "startUp",                  /* The text name assigned to the task - for debug only as it is not used by the kernel. */
            configMINIMAL_STACK_SIZE,   /* The size of the stack to allocate to the task. */
            &main_context, 					    /* The parameter passed to the task - not used in this case. */
            STARTUP_TASK_PRIORITY, 	    /* The priority assigned to the task. */
            NULL );

    /* Start the tasks and timer running. */
	vTaskStartScheduler(); // Never return
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