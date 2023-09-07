#include "test.h"

void test_task(void *pvParameters)
{
    printf("TEST task\n");
    while (1) {
        // Add function to test here
        interface_publish("Sujet", 14);


        vTaskDelay(5000); // Publish every 5s
    }
}
