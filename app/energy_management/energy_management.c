#include "energy_management.h"
#include "context.h"

#define STATUS_ENERGY_TOPIC         ("Status Contacteurs")
#define PV_VOLTAGE_TOPIC_12         ("Tension PV1_2")
#define PV_VOLTAGE_TOPIC_34         ("Tension PV3_4")
#define BATTERY_VOLTAGE_1           ("Tension Batterie 1")
#define BATTERY_VOLTAGE_12          ("Tension Batterie 1_2")
#define PV_CURRENT_TOPIC_12         ("Courant PV1_2")
#define PV_CURRENT_TOPIC_34         ("Courant PV3_4")
#define PV_POWER_TOPIC                  ("Puissance PV")

#define INTRUMENTATION_CURRENT_TOPIC    ("Entree Instrumentation")
#define INTRUMENTATION_POWER_TOPIC      ("Puissance Instrumentation")
#define BATTERY_CURRENT_TOPIC           ("Sortie Batterie")

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

#define BOTH_BATTERY_VOLTAGE_INDEX  (1)

#define BATTERY_OVERVOLTAGE         (28.5)
#define BATTERY_HIGH_VOLTAGE        (26.6)
#define BATTERY_LOW_VOLTAGE         (25.0)
#define BATTERY_VERY_LOW_VOLTAGE    (24.0)
#define BATTERY_LOAD_VOLTAGE_DROP   (1.0)

#define DELAY_30_MIN                (30*60*1000)
uint32_t discharge_reenable_time = 0;

#define SUFFICIANT_IRRADIANCE_POWER (800)

#define MEASUREMENTS_PERIOD_MS      (5*1000)
#define PUBLISH_PERIOD_MS           (60*1000)
#define MINMUM_DISCHARGE_TIME_MS    (30*1000)
#define OVERCHARGED_DISCHARGE_TIME_MS   (5*60*1000)

#define CONTACTOR_STATUS_TOPIC  ("Status contacteur")
static void connect_contactor(void) {
    gpio_put(LOAD_RELAY_GPIO, true);
    interface_publish(CONTACTOR_STATUS_TOPIC, 1.0);
}

static void disconnect_contactor(void){
    gpio_put(LOAD_RELAY_GPIO, false);
    interface_publish(CONTACTOR_STATUS_TOPIC, 0.0);
}

static bool battery_need_discharge(float voltage)
{
    if (voltage >= BATTERY_HIGH_VOLTAGE) {
        return true;
    }
    return false;
}

static bool battery_low_charge(float voltage)
{
    if (voltage <= BATTERY_LOW_VOLTAGE) {
            return true;
    }
    return false;
}

static bool battery_good(float voltage)
{
    if (voltage >= BATTERY_LOW_VOLTAGE
        && voltage <= (BATTERY_HIGH_VOLTAGE - BATTERY_LOAD_VOLTAGE_DROP)) {
        return true;
    }
    return false;
}

static bool battery_criticaly_low(float voltage)
{
    if (voltage <= BATTERY_VERY_LOW_VOLTAGE) {
        return true;
    }
    return false;
}

static bool sufficient_irradiance(void)
{
    if (get_instant_power_PV() >= SUFFICIANT_IRRADIANCE_POWER) {
        return true;
    }
    return false;
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

void init_energy(void)
{
    gpio_init(LOAD_RELAY_GPIO);
    gpio_set_dir(LOAD_RELAY_GPIO, GPIO_OUT);
    disconnect_contactor();
}

static float battery_voltage[NB_BAT] = {0};
static float PV_voltage[NB_PV] = {0};
static float battery_bus_voltage = 0;
static float PV_current[NB_PV] = {0};
static float instru_current = 0;
static float battery_current = 0;

void take_and_publish_measurement() {
    static uint32_t publish_measurements = 0;
    publish_measurements++;
    for (uint8_t i = 0; i < NB_PV; i++)
    {
        PV_voltage[i] = get_PV_voltage(i);
        PV_current[i] = get_PV_current(i);
        
        if (!(publish_measurements % (PUBLISH_PERIOD_MS / MEASUREMENTS_PERIOD_MS))) {
            interface_publish(pv_voltage_topic[i], PV_voltage[i]);
            interface_publish(pv_current_topic[i], PV_current[i]);
            interface_publish(PV_POWER_TOPIC, PV_current[0]*PV_voltage[0] + PV_current[1]*PV_voltage[1]);
        }
    }

    for (uint8_t i = 0; i < NB_BAT; i++)
    {
        battery_voltage[i] = get_battery_voltage(i*2 + 1);

        if (!(publish_measurements % (PUBLISH_PERIOD_MS / MEASUREMENTS_PERIOD_MS))) {
            interface_publish(battery_voltage_topic[i], battery_voltage[i]);
        }
    }

    instru_current = get_instrumentation_current();
    battery_current = get_battery_current();

    if (!(publish_measurements % (PUBLISH_PERIOD_MS / MEASUREMENTS_PERIOD_MS))) {
        interface_publish(INTRUMENTATION_CURRENT_TOPIC, instru_current);
        interface_publish(INTRUMENTATION_POWER_TOPIC, instru_current*battery_voltage[1]);
        interface_publish(BATTERY_CURRENT_TOPIC, battery_current);
    }
}

void discharge_control(discharge_status_t discharge_state)
{
    
    switch (discharge_state) {
    case UNLIMITED_DISCHARGE:
        // Keep discharging until state change    
        connect_contactor();
        break;
    case SUNNY_DISCHARGE:
        // Discharge if sunny and check every 30 minutes
        static uint32_t keep_load_connected_30_min = 0;
        uint32_t now = to_ms_since_boot(get_absolute_time());
        if (sufficient_irradiance() && keep_load_connected_30_min < now) {
            connect_contactor();
            keep_load_connected_30_min =  now + DELAY_30_MIN;
        } else if (keep_load_connected_30_min > now) {
            // Keep load connected
            connect_contactor();
        } else {
            disconnect_contactor();
        }
        
        break;
    case NO_DISCHARGE:
    default:
        // Disconnect load
        disconnect_contactor();
        break;
    }
}

void enery_management(void *pvParameters)
{
    main_context_t *context = (main_context_t*)pvParameters;
    static energy_status_t energy_state = ENERGY_INIT;
    static energy_status_t last_energy_state = ENERGY_ERROR;

    last_energy_state = energy_state;

    while(1) {
        take_and_publish_measurement(false);

        /*** Energy state machine ***/

        switch (energy_state) {
        case ENERGY_INIT:

            energy_state = ENERGY_IDLE;

            break;

        case ENERGY_IDLE:
            uint32_t now = to_ms_since_boot(get_absolute_time());
            if (battery_need_discharge(battery_voltage[BOTH_BATTERY_VOLTAGE_INDEX]) && (now >= discharge_reenable_time)) {
                energy_state = OVERCHARGED;

            } else if (battery_criticaly_low(battery_voltage[BOTH_BATTERY_VOLTAGE_INDEX])) {
                energy_state = POWER_SAVING;

            } else if (battery_low_charge(battery_voltage[BOTH_BATTERY_VOLTAGE_INDEX])) {
                energy_state = LOAD_SHEDDING;

            } else if (battery_good(battery_voltage[BOTH_BATTERY_VOLTAGE_INDEX]) || (now < discharge_reenable_time)){
                energy_state = NORMAL_USE;
            }

            // Publish on state change
            if (last_energy_state != energy_state) {
                interface_publish(STATUS_ENERGY_TOPIC, energy_state);
            }

            break;

        case LOAD_SHEDDING:
            // Disconnect load
            disconnect_contactor();

            // Disable irrigation
            context->irrigation_enable = false;
            
            // Enable motor drive
            context->motor_drive_enable = true;

            energy_state = ENERGY_IDLE;

            break;

        case POWER_SAVING:
            // Disconnect load
            disconnect_contactor();

            // Disable irrigation
            context->irrigation_enable = true;

            // Turn off motor drive
            context->motor_drive_enable = false; // Will hit limit switch and be turned off

            energy_state = ENERGY_IDLE;

            break;

        case NORMAL_USE:
            // Disconnect load
            disconnect_contactor();

            // Enable irrigation
            context->irrigation_enable = true;

            // Enable motor drive
            context->motor_drive_enable = true;

            energy_state = ENERGY_IDLE;

            break;

        case OVERCHARGED:
            // Enable irrigation
            context->irrigation_enable = true;

            // Enable motor drive
            context->motor_drive_enable = true;

            // Manage load
            discharge_status_t discharge_state;
            if (morning() || battery_voltage[BOTH_BATTERY_VOLTAGE_INDEX] > BATTERY_OVERVOLTAGE) {
                discharge_state = UNLIMITED_DISCHARGE;
            } else if (daytime()) {
                discharge_state = SUNNY_DISCHARGE;
            } else {
                discharge_state = NO_DISCHARGE;
            }
            discharge_control(discharge_state);

            break;

        case ENERGY_ERROR:
            // Print error message on thingsboard
            energy_state = ENERGY_INIT;

            break;
        }
    // Do not update when next state is idle
    last_energy_state = (energy_state == ENERGY_IDLE) ? last_energy_state : energy_state;
    vTaskDelay(MEASUREMENTS_PERIOD_MS);
    }
}
