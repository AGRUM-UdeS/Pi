#include "utils.h"

#define USB_CONNECTION_DELAY_S 5

void usb_delay(void)
{
    printf("\n");
    for (size_t i = 0; i <= USB_CONNECTION_DELAY_S; i++)
    {
        sleep_ms(1000);
        printf("%d..", (USB_CONNECTION_DELAY_S - i));
    }
    printf("\n");
}
