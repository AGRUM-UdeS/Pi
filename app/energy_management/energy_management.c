#include "energy_management.h"

#include "sensors.h"

void init_energy_management(void)
{
    // TODO
}

float get_instant_power_PV(void)
{
    float PV_voltage[NB_PV], PV_current[NB_PV];
    for (size_t i = 0; i < NB_PV; i++)
    {
        PV_voltage[i] = get_PV_voltage(i);
        PV_current[i] = get_PV_current(i);
    }

    // Convert voltage and current to power

    // Return instantaneous power
}
