#include "energy_management.h"
#include "context.h"

#define STATUS_ENERGY_TOPIC         ("Status Contacteurs")
#define PV_VOLTAGE_TOPIC_12         ("Tension PV1_2")
#define PV_VOLTAGE_TOPIC_34         ("Tension PV3_4")
#define BATTERY_VOLTAGE_1           ("Tension PV1_2")
#define BATTERY_VOLTAGE_12          ("Tension PV3_4")
#define PV_CURRENT_TOPIC_12         ("Courant PV1_2")
#define PV_CURRENT_TOPIC_34         ("Courant PV3_4")

char *pv_voltage_topic[] = {
    PV_VOLTAGE_TOPIC_12,
    PV_VOLTAGE_TOPIC_34
};

char *battery_voltage_topic[] = {
    BATTERY_VOLTAGE_1,
    BATTERY_VOLTAGE_12
};

char *pv_current_topic[] = {
    PV_CURRENT_TOPIC_12,
    PV_CURRENT_TOPIC_34
};

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

static bool load_is_connected(void)
{
    if (1 /* Read load relay pin state */) {
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
    main_context_t *context = (main_context_t*)pvParameters;
    static energy_status_t energy_state = ENERGY_INIT;
    static energy_status_t last_energy_state = ENERGY_ERROR;

    last_energy_state = energy_state;

    // Start measurement timer
    if (!add_repeating_timer_ms(-MEASUREMENTS_PERIOD_MS, meas_callback, NULL, &measure_timer)) {
        printf("Failed to add energy timer\n");
    }

    float battery_voltage[NB_BAT] = {0};
    float PV_voltage[NB_PV] = {0};
    float PV_current[NB_PV] = {0};
    float instru_current = 0;
    float battery_current = 0;

    if (time_to_measure()) {
         energy_state = ENERGY_MEASUREMENT;
    }

    switch (energy_state) {
    case ENERGY_INIT:

        energy_state = ENERGY_MEASUREMENT;

        break;

    case ENERGY_MEASUREMENT:
        interface_publish(STATUS_ENERGY_TOPIC, ENERGY_MEASUREMENT);

        // Take and publish measurement
        for (size_t i = 0; i < NB_PV; i++)
        {
            PV_voltage[i] = get_PV_voltage(i);
            interface_publish(pv_voltage_topic[i], PV_voltage[i]);
            PV_current[i] = get_PV_current(i);
            interface_publish(pv_current_topic[i], PV_current[i]);
        }

        for (size_t i = 0; i < NB_BAT; i++)
        {
            battery_voltage[i] = get_battery_voltage(i+2);
            interface_publish(battery_voltage_topic[i], battery_voltage[i]);
        }

        instru_current = get_instrumentation_current();
        battery_current = get_battery_current();

        if (battery_is_overcharged(battery_voltage, NB_PV)) {
            interface_publish(STATUS_ENERGY_TOPIC, OVERCHARGED);
            energy_state = OVERCHARGED;

        } else if (battery_is_ok(battery_voltage, NB_PV)) {
            interface_publish(STATUS_ENERGY_TOPIC, NORMAL_USE);
            energy_state = NORMAL_USE;

        } else if (battery_is_low(battery_voltage, NB_PV)) {
            interface_publish(STATUS_ENERGY_TOPIC, POWER_SAVING);
            energy_state = POWER_SAVING;

        } else if (battery_is_empty(battery_voltage, NB_PV)) {
            interface_publish(STATUS_ENERGY_TOPIC, LOAD_SHEDDING);
            energy_state = LOAD_SHEDDING;

        } else {
            energy_state = ENERGY_ERROR;
        }

        break;

    case LOAD_SHEDDING:
        // Disable irrigation
        if (load_is_connected()) {
            // Disconnect inverter
            gpio_put(LOAD_RELAY_GPIO, false);
        }
        context->irrifation_enable = false;

        break;

    case POWER_SAVING:
        if (load_is_connected()) {
            // Disconnect inverter
            gpio_put(LOAD_RELAY_GPIO, false);
        }
        context->irrifation_enable = false;

        break;

    case NORMAL_USE:
        if (!(load_is_connected())) {
            // Connect inverter
            gpio_put(LOAD_RELAY_GPIO, true);
        }
        context->irrifation_enable = true;

        break;

    case OVERCHARGED:
        if (!(load_is_connected())) {
            // Connect load
            gpio_put(LOAD_RELAY_GPIO, true);
        }
        context->irrifation_enable = true;

        break;

    case ENERGY_ERROR:
        // Print error message on thingsboard
        energy_state = ENERGY_INIT;

        break;
    }
}
