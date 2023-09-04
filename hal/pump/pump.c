#include "pump.h"

#define IO_IRRIGATION_ADDRESS   (IO_address_0)
#define PUMP1_PIN  (4)
#define PUMP2_PIN  (5)

void disable_all_pump(void)
{
    // Clear pin before setting it as output
    // because default state is HIGH
    IO_clear_pin(IO_IRRIGATION_ADDRESS, PUMP1_PIN);
    IO_clear_pin(IO_IRRIGATION_ADDRESS, PUMP2_PIN);
    
    // Set pump pin as output on IO expander
    IO_set_as_output(IO_IRRIGATION_ADDRESS, PUMP1_PIN);
    IO_set_as_output(IO_IRRIGATION_ADDRESS, PUMP2_PIN);
}
