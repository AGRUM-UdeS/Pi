#include "energy_management.h"

#include "sensors.h"

#define PV_VOLTAGE_TOPIC        ("Tension PV")
#define PV_CURRENT_TOPIC        ("Courant PV")
// TODO: definir autre mesure

// TODO: Modify voltage
#define BATTERY_OVERCHARGED_VOLTAGE (27.0)
#define BATTERY_FULL_VOLTAGE        (26.0)
#define BATTERY_LOW_VOLTAGE         (23.0)
#define BATTERY_EMPTY_VOLTAGE       (22.0)

#define MEASUREMENTS_PERIOD_MS  (10*1000)
static repeating_timer_t measure_timer;
static bool energy_measurement_flag = false;

static bool meas_callback(repeating_timer_t *rt)
{
    energy_measurement_flag = true;
    return energy_measurement_flag;
}

static bool time_to_measure(void)
{
    if (energy_measurement_flag == true) {
        energy_measurement_flag = false;
        return true;
    } else {
        return false;
    }
}

static bool battery_is_overcharged(float voltage[], uint16_t size)
{
    // Detect first overvoltage battery
    for (size_t i = 0; i < size; i++) {
        if (voltage[i] >= BATTERY_OVERCHARGED_VOLTAGE) {
            return true;
        }
    }
    return false;
}

static bool battery_is_ok(float voltage[], uint16_t size)
{
    // Make sure battery voltage is below limit
    for (size_t i = 0; i < size; i++) {
        if (voltage[i] <= BATTERY_FULL_VOLTAGE) {
            return true;
        }
    }
    return false;
}

static bool battery_is_low(float voltage[], uint16_t size)
{
    // Detect empty battery
    for (size_t i = 0; i < size; i++) {
        if (voltage[i] <= BATTERY_LOW_VOLTAGE) {
            return true;
        }
    }
    return false;
}

static bool battery_is_empty(float voltage[], uint16_t size)
{
    // Detect empty battery
    for (size_t i = 0; i < size; i++) {
        if (voltage[i] <= BATTERY_LOW_VOLTAGE) {
            return true;
        }
    }
    return false;
}

static bool inverter_is_connected(void)
{
    if (1 /* Read inverter ctrl pin state */) {
        return true;
    } else {
        return false;
    }
}

static bool battery_is_connected(void)
{
    if (1 /* Read battery relay pin state */) {
        return true;
    } else {
        return false;
    }
}

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

void enery_management(void *pvParameters)
{
    static energy_status_t energy_state = ENERGY_INIT;
    static energy_status_t last_energy_state = ENERGY_ERROR;

    last_energy_state = energy_state;

    // Start measurement timer
    if (!add_repeating_timer_ms(-MEASUREMENTS_PERIOD_MS, meas_callback, NULL, &measure_timer)) {
        printf("Failed to add energy timer\n");
    }

    static float battery_voltage[NB_PV] = {0};

    if (time_to_measure()) {
         energy_state = ENERGY_MEASUREMENT;
    }

    switch (energy_state) {
    case ENERGY_INIT:
        // Init relays states

        // Validate states

        energy_state = ENERGY_MEASUREMENT;

        break;

    case ENERGY_MEASUREMENT:
        // Take and publish measurement
        /* Test */
        for (size_t i = 0; i < NB_PV; i++)
        {
            battery_voltage[i] = BATTERY_LOW_VOLTAGE + 1;
        }

        if (battery_is_overcharged(battery_voltage, NB_PV)) {
            energy_state = OVERCHARGED;
        } else if (battery_is_ok(battery_voltage, NB_PV)) {
            energy_state = NORMAL_USE;
        } else if (battery_is_low(battery_voltage, NB_PV)) {
            energy_state = POWER_SAVING;
        } else if (battery_is_empty(battery_voltage, NB_PV)) {
            energy_state = LOAD_SHEDDING;
        } else {
            energy_state = ENERGY_ERROR;
        }

        break;

    case LOAD_SHEDDING:
        // Disable irrigation
        if (inverter_is_connected()) {
            // Disconnect inverter
        }

        if (!(battery_is_connected())) {
            // Connect battery
        }

        break;

    case POWER_SAVING:
        if (inverter_is_connected()) {
            // Disconnect inverter
        }

        if (!(battery_is_connected())) {
            // Connect battery
        }

        break;

    case NORMAL_USE:
        if (!(inverter_is_connected())) {
            // Connect inverter
        }

        if (!(battery_is_connected())) {
            // Connect battery
        }

        break;

    case OVERCHARGED:
        if (!(inverter_is_connected())) {
            // Connect inverter
        }

        if (battery_is_connected()) {
            // Disconnect battery
        }

        break;

    case ENERGY_ERROR:
        // Print error message on thingsboard
        energy_state = ENERGY_INIT;

        break;
    }
}
