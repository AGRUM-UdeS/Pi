#include "utils.h"

void usb_delay(uint8_t delay_s)
{
    printf("\n");
    for (size_t i = 0; i <= delay_s; i++)
    {
        sleep_ms(1000);
        printf("%d..", (delay_s - i));
    }
    printf("\n");
}
