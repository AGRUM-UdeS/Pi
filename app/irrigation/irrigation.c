#include "irrigation.h"

#define TEMP_TOPIC_1              ("Temperature avec PV 1")
#define HUMIDITY_TOPIC_1          ("Humidite avec PV 1")
#define TEMP_TOPIC_2              ("Temperature avec PV 2")
#define HUMIDITY_TOPIC_2          ("Humidite avec PV 2")
#define TEMP_TOPIC_3              ("Temperature sans PV 1")
#define HUMIDITY_TOPIC_3          ("Humidite sans PV 1")
#define TEMP_TOPIC_4              ("Temperature sans PV 2")
#define HUMIDITY_TOPIC_4          ("Humidite sans PV 2")

char *temperature_topic[] = {
    TEMP_TOPIC_1,
    TEMP_TOPIC_2,
    TEMP_TOPIC_3,
    TEMP_TOPIC_4
};

char *humidity_topic[] = {
    HUMIDITY_TOPIC_1,
    HUMIDITY_TOPIC_2,
    HUMIDITY_TOPIC_3,
    HUMIDITY_TOPIC_4
};

#define MEASUREMENTS_PERIOD_MS  (1*5*1000)

static repeating_timer_t measure_timer;
static bool measure_flag = false;

static bool meas_callback(repeating_timer_t *rt)
{
    measure_flag = true;
    return measure_flag;
}

void init_irrigation(void)
{
    init_water_level_sensors();
    init_pump();
    init_valve();
    // negative timeout means exact delay (rather than delay between callbacks)
    if (!add_repeating_timer_ms(-MEASUREMENTS_PERIOD_MS, meas_callback, NULL, &measure_timer)) {
        printf("Failed to add irrigation timer\n");
    }
}

irrigation_status_t irrigation_sm(void)
{
    static irrigation_status_t irrigation_state = IRRIGATION_IDLE;

    switch (irrigation_state) {
    case IRRIGATION_IDLE:
        if (measure_flag) {
            measure_flag = false;
            irrigation_state = IRRIGATION_MEASURING;
        }

        break;

    case IRRIGATION_MEASURING:
        SHT_measure_t meas[ENVIRO_SENSOR_NB];

        // Read every enviro sensor and publish data
        for (size_t i = 0; i < ENVIRO_SENSOR_NB; i++) {
            meas[i] = read_temp_humidity(enviro_sensor_location[i]);

            if (meas[i].meas_ok) {
                interface_publish(temperature_topic[i], meas[i].temp);
                sleep_ms(50);
                interface_publish(humidity_topic[i], meas[i].humidity);
            } else {
                printf("Failed to take temp&humidity measurements (%d)\n", i);
            }
        }
        
        irrigation_state = IRRIGATION_IDLE;
        break;

    case IRRIGATION_PUMPING:

        break;

    case IRRIGATION_IRRIGATING:

        break;

    case IRRIGATION_ERROR:

        break;
    }

    return irrigation_state;
}
