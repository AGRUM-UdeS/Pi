#include "irrigation.h"

#include <math.h>

#define TEMP_TOPIC              ("Temperature")
#define HUMIDITY_TOPIC          ("Humidite")
//#define MEASUREMENTS_PERIOD_MS  (1*60*1000)
#define MEASUREMENTS_PERIOD_MS  (60*1000)

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
        SHT_measure_t meas = read_temp_humidity();

        // Test without sensor
        meas.meas_ok = true;
        static float i = 0;
        meas.temp = 20 + 4 * cos(2.0 * M_PI * (i++/50.0) + 4);
        meas.humidity = 35 + 1 * sin(2.0 * M_PI * (i++/20.0));

        if (meas.meas_ok) {
            interface_publish(TEMP_TOPIC, meas.temp);
            interface_publish(HUMIDITY_TOPIC, meas.humidity);
        } else {
            printf("Failed to take temp&humidity measurements\n");
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
