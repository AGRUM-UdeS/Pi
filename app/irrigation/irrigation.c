#include "irrigation.h"
#include "context.h"

#define IRRIGATION_STATUS         ("Status irrigation")

// Thingsboard irrigation measurement topic
#define TEMP_TOPIC_1              ("Temperature avec PV")
#define HUMIDITY_TOPIC_1          ("Humidite avec PV")
// #define TEMP_TOPIC_2              ("Temperature avec PV 2")
// #define HUMIDITY_TOPIC_2          ("Humidite avec PV 2")
#define TEMP_TOPIC_2              ("Temperature sans PV")
#define HUMIDITY_TOPIC_2          ("Humidite sans PV")
// #define TEMP_TOPIC_4              ("Temperature sans PV 2")
// #define HUMIDITY_TOPIC_4          ("Humidite sans PV 2")
#define HUMIDITY_SOIL_1           ("Humidite Sol 1")
#define HUMIDITY_SOIL_2           ("Humidite Sol 2")
#define HUMIDITY_SOIL_3           ("Humidite Sol 3")

char *temperature_topic[] = {
    TEMP_TOPIC_1,
    TEMP_TOPIC_2
    // TEMP_TOPIC_3,
    // TEMP_TOPIC_4
};

char *humidity_topic[] = {
    HUMIDITY_TOPIC_1,
    HUMIDITY_TOPIC_2,
    // HUMIDITY_TOPIC_3,
    // HUMIDITY_TOPIC_4
};

char *soil_humidity_topic[] = {
    HUMIDITY_SOIL_1,
    HUMIDITY_SOIL_2,
    HUMIDITY_SOIL_3
};

// Sensor IO defines
#define HUMIDITY_SOL_1              (0)
#define HUMIDITY_SOL_2              (1)
#define HUMIDITY_SOL_3              (2)

#define BARREL_WATER_LEVEL_PIN  (0)
#define BAC_WATER_LEVEL_PIN     (1)
#define VALVE_SPRINKLER         (2)
#define VALVE_SOAKER            (3)
#define PUMP_IRRIGATION         (5)
#define PUMP_BAC2BARREL         (4)
#define LED_BARIL               (6)
#define LED_BAC                 (7)

// State flag
static bool irrigation_watering_flag = false;
static bool irrigation_soaking_flag = false;
static bool irrigation_measurement_flag = false;

// Irrigation timing
#define WATERING_DURATION_MS    pdMSTOTICKS(10*60*1000)
#define BAC2BARREL_DURATION_MS  pdMSTOTICKS(30*1000)
#define SOAKING_DURATION_MS     pdMSTOTICKS(10*60*1000)

#define MEASUREMENTS_PERIOD_MS  (2*60*1000)

static repeating_timer_t measure_timer;

static bool meas_callback(repeating_timer_t *rt)
{
    irrigation_measurement_flag = true;
    return irrigation_measurement_flag;
}

static bool time_to_measure(void)
{
    if (irrigation_measurement_flag == true) {
        irrigation_measurement_flag = false;
        return true;
    }

    return false;
}

static void watering_alarm_callback(void)
{
    irrigation_watering_flag = true;
}

// Irrigation state machine
void irrigation_management(void *pvParameters)
{
    main_context_t *context = (main_context_t*)pvParameters;
    irrigation_status_t irrigation_state = IRRIGATION_INIT;
    irrigation_status_t last_irrigation_state = IRRIGATION_ERROR;

    // negative timeout means exact delay (rather than delay between callbacks)
    if (!add_repeating_timer_ms(-MEASUREMENTS_PERIOD_MS, meas_callback, NULL, &measure_timer)) {
        printf("Failed to add irrigation timer\n");
    }

    while(1) {
        last_irrigation_state = irrigation_state;

        switch (irrigation_state) {
            case IRRIGATION_INIT:
                // Close valves
                close_all_valve();

                // Close pumps
                disable_all_pump();

                // Init level sensor as input
                init_water_level_sensors();

                // Init leds
                clear_all_irigation_led();

                irrigation_state = IRRIGATION_MEASUREMENT;
                break;

            case IRRIGATION_IDLE:
                // interface_publish(IRRIGATION_STATUS, IRRIGATION_IDLE);
                close_all_valve();
                disable_all_pump();
                clear_all_irigation_led();

                if (barrel_is_empty()) {
                    turn_on_irrigation_led(LED_BAC);
                } else {
                    turn_off_irrigation_led(LED_BAC);
                }

                if (time_to_measure()) {
                    irrigation_state = IRRIGATION_MEASUREMENT;
                } else if (morning_irrigation() && !barrel_is_empty() && context->irrigation_enable) {
                    clear_irrigation_flag();
                    irrigation_state = IRRIGATION_WATERING;
                } else if (bac_is_full() && context->irrigation_enable) {
                    irrigation_state = IRRIGATION_RESERVOIR2BARREL;
                } else if (irrigation_soaking_flag && context->irrigation_enable) {
                    irrigation_state = IRRIGATION_SOAKING;
                } else {
                    irrigation_state = IRRIGATION_IDLE;
                }
                break;

            case IRRIGATION_MEASUREMENT:
                interface_publish(IRRIGATION_STATUS, IRRIGATION_MEASUREMENT);
                SHT_measure_t meas[ENVIRO_SENSOR_NB];

                // Read every enviro sensor and publish data
                for (size_t i = 0; i < ENVIRO_SENSOR_NB; i++) {
                    meas[i] = read_temp_humidity(enviro_sensor_location[i]);

                    if (meas[i].meas_ok) {
                        interface_publish(temperature_topic[i], meas[i].temp);
                        interface_publish(humidity_topic[i], meas[i].humidity);
                    } else {
                        printf("Failed to take temp&humidity measurements (%d)\n", i);
                    }
                }

                for (size_t i = 1; i < SOIL_HUMIDITY_SENSOR_NB + 1; i++) {
                    // Read soil humidity
                    float soil_humidity[SOIL_HUMIDITY_SENSOR_NB];
                    read_soil_humidity(i, &(soil_humidity[i]));
                    // Publish soil humidity
                    interface_publish(soil_humidity_topic[i], soil_humidity[i]);
                }

                //JC : Weather
                /*
                    JC : 
                    Ajouter Conditions pour lever les différents flags en fonction des mesures environnementals et météo
                    
                    Différents flags:
                    -irrigation_watering_flag
                    -irrigation_waterlevel_trigger
                    -irrigation_soaking_flag
                */
                interface_publish(IRRIGATION_STATUS, IRRIGATION_IDLE);
                irrigation_state = IRRIGATION_IDLE;
                break;

            case IRRIGATION_WATERING:
                interface_publish(IRRIGATION_STATUS, IRRIGATION_WATERING);

                // Spray water
                open_valve(VALVE_SPRINKLER);
                enable_pump(PUMP_IRRIGATION);
                
                // Wait for plants to get wet
                vTaskDelay(WATERING_DURATION_MS);

                // Stop watering
                disable_pump(PUMP_IRRIGATION);
                close_valve(VALVE_SPRINKLER);

                interface_publish(IRRIGATION_STATUS, IRRIGATION_IDLE);
                irrigation_state = IRRIGATION_IDLE;
                break;

            case IRRIGATION_RESERVOIR2BARREL:
                interface_publish(IRRIGATION_STATUS, IRRIGATION_RESERVOIR2BARREL);

                turn_on_irrigation_led(LED_BAC);

                enable_pump(PUMP_BAC2BARREL);

                vTaskDelay(BAC2BARREL_DURATION_MS);
                
                disable_pump(PUMP_BAC2BARREL);

                turn_off_irrigation_led(LED_BAC);

                interface_publish(IRRIGATION_STATUS, IRRIGATION_IDLE);
                irrigation_state = IRRIGATION_IDLE;
                break;

            case IRRIGATION_SOAKING:
                interface_publish(IRRIGATION_STATUS, IRRIGATION_SOAKING);

                // Start soaking
                open_valve(VALVE_SOAKER);
                enable_pump(PUMP_IRRIGATION);
                
                // Wait for plants to get soaked
                vTaskDelay(SOAKING_DURATION_MS);

                // Stop soaking
                disable_pump(PUMP_IRRIGATION);
                close_valve(VALVE_SOAKER);

                interface_publish(IRRIGATION_STATUS, IRRIGATION_IDLE);
                irrigation_state = IRRIGATION_IDLE;
                break;

            case IRRIGATION_ERROR:
                interface_publish(IRRIGATION_STATUS, IRRIGATION_ERROR);
                // Print error message on thingsboard
                irrigation_state = IRRIGATION_INIT;

            break;
        }
        last_irrigation_state = irrigation_state;
        context->irrigation_status = irrigation_state;
        vTaskDelay(500);
    }
}
