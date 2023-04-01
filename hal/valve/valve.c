#include "valve.h"

#define IO_IRRIGATION_ADDRESS   (IO_address_0)
#define VALVE1_PIN  (2)
#define VALVE2_PIN  (3)

void init_valve(void)
{
    // Clear pin before setting them as output
    // because default state is HIGH
    IO_clear_pin(IO_IRRIGATION_ADDRESS, VALVE1_PIN);
    IO_clear_pin(IO_IRRIGATION_ADDRESS, VALVE2_PIN);

    // Set valve pin as output on IO expander
    uint8_t byte = (1 << VALVE1_PIN) | (1 << VALVE2_PIN);
    IO_set_as_output(IO_IRRIGATION_ADDRESS, byte);
}
