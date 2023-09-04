#include "valve.h"

#define IO_IRRIGATION_ADDRESS   (IO_address_0)
#define VALVE1_PIN  (2)
#define VALVE2_PIN  (3)

void close_all_valve(void)
{
    // Clear pin before setting them as output
    // because default state is HIGH
    IO_clear_pin(IO_IRRIGATION_ADDRESS, VALVE1_PIN);
    IO_clear_pin(IO_IRRIGATION_ADDRESS, VALVE2_PIN);

    // Set valve pin as output on IO expander
    IO_set_as_output(IO_IRRIGATION_ADDRESS, VALVE1_PIN);
    IO_set_as_output(IO_IRRIGATION_ADDRESS, VALVE2_PIN);
}

valve_state_t open_valve(uint8_t valve_id)
{
    IO_status_t status = IO_set_pin(IO_IRRIGATION_ADDRESS, valve_id);

    if (status == IO_ok) {
        return VALVE_OPEN;
    } else {
        return VALVE_UNKNOWN;
    }
}

valve_state_t close_valve(uint8_t valve_id)
{
    IO_status_t status = IO_clear_pin(IO_IRRIGATION_ADDRESS, valve_id);

    if (status == IO_ok) {
        return VALVE_CLOSED;
    } else {
        return VALVE_UNKNOWN;
    }
}