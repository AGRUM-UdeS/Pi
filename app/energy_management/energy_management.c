#include "energy_management.h"

#include "sensors.h"

float get_instant_power_PV(void)
{
    float PV_voltage[NB_PV], PV_current[NB_PV], PV_power = 0;
    for (size_t i = 0; i < NB_PV; i++)
    {
        PV_voltage[i] = get_PV_voltage(i);
        PV_current[i] = get_PV_current(i);
        PV_power += (PV_voltage[i] * PV_current[i]);
    }
    return PV_power;
}

energy_status_t enery_management(void)
{
    static energy_status_t energy_state = ENERGY_IDLE;

    switch (energy_state) {
    case ENERGY_INIT:
        // Init relays states

        // Validate states

        energy_state = ENERGY_MEASUREMENT;
        break;

    case ENERGY_IDLE:
        if (time_to_measure()) {
            energy_state = ENERGY_MEASUREMENT;
        }

        break;

    case ENERGY_MEASUREMENT:
        // Take and publish measurement

        if (battery_is_full()) {
            energy_state = INVERTER_CONNECT;
        } else if (battery_is_ok()) {
            energy_state = INVERTER_DISCONNECT;
        } else {
            energy_state = ENERGY_ERROR;
        }

        break;

    case INVERTER_DISCONNECT:
        if (inverter_is_connected()) {
            // Disconnect inverter

        }

        energy_state = BATTERY_CONNECT;

        break;

    case INVERTER_CONNECT:
        if (!(inverted_is_connected())) {
            // Connect inverter

        }

        if (battery_is_overcharged()) {
            energy_state = BATTERY_DISCONNECT;
        } else {
            // Let battery discharged
            energy_state = ENERGY_IDLE;
        }

        break;

    case BATTERY_DISCONNECT:
        if (battery_is_connected()) {
            // Disconnect battery
        }
        
        energy_state = ENERGY_IDLE;

        break;

    case BATTERY_CONNECT:
        if (!(battery_is_connected())) {
            // Connect battery
        }
        if (battery_is_empty()) {
            energy_state = LOAD_SHEDDING;
        } else (
            energy_state = ENERGY_IDLE;
        )

        break;

    case LOAD_SHEDDING:
        // Take measurements since you can hang here
        if (time_to_measure()) {
            // Take measurement and publish them
            
        }

        if (battery_is_ok()) {
            energy_state = ENERGY_IDLE;
        }

        break;

    case ENERGY_ERROR:  
        // Print error message on thingsboard
        energy_state = ENERGY_INIT;

        break;
    }

    return irrigation_state;
}
