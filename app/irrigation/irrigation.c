#include "irrigation.h"
#include "context.h"

#define TEMP_TOPIC_1              ("Temperature avec PV 1")
#define HUMIDITY_TOPIC_1          ("Humidite avec PV 1")
#define TEMP_TOPIC_2              ("Temperature avec PV 2")
#define HUMIDITY_TOPIC_2          ("Humidite avec PV 2")
#define TEMP_TOPIC_3              ("Temperature sans PV 1")
#define HUMIDITY_TOPIC_3          ("Humidite sans PV 1")
#define TEMP_TOPIC_4              ("Temperature sans PV 2")
#define HUMIDITY_TOPIC_4          ("Humidite sans PV 2")
#define HUMIDITY_SOIL_1           ("Humidite sol 1")
#define HUMIDITY_SOIL_2           ("Humidite sol 2")
#define HUMIDITY_SOIL_3           ("Humidite sol 3")

#define HUMIDITY_SOL_1              (0)
#define HUMIDITY_SOL_2              (1)
#define HUMIDITY_SOL_3              (2)

static bool irrigation_watering_flag = false;
static bool irrigation_soaking_flag = false;
static bool irrigation_waterlevel_trigger = false;
static bool irrigation_measurement_flag = false;

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

char *soil_humidity_topic[] = {
    HUMIDITY_SOIL_1,
    HUMIDITY_SOIL_2,
    HUMIDITY_SOIL_3
};

#define MEASUREMENTS_PERIOD_MS  (5*60*1000)

static repeating_timer_t measure_timer;
static bool measure_flag = false;

static bool meas_callback(repeating_timer_t *rt)
{
    measure_flag = true;
    return measure_flag;
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

                irrigation_state = IRRIGATION_IDLE;
                break;

            case IRRIGATION_IDLE:
                close_all_valve();
                disable_all_pump();
                clear_all_irigation_led();

                if (time_to_measure()) {
                    irrigation_state = IRRIGATION_MEASUREMENT;
                }
                else if (morning_irrigation()) {
                    clear_irrigation_flag();
                    irrigation_state = IRRIGATION_WATERING;
                } 
                else if (irrigation_waterlevel_trigger) {
                    irrigation_state = IRRIGATION_RESERVOIR2BARREL;
                }
                else if (irrigation_soaking_flag) {
                    irrigation_state = IRRIGATION_SOAKING;
                }
                else {
                    irrigation_state = IRRIGATION_IDLE;
                }
                break;

            case IRRIGATION_MEASUREMENT:
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

                for (size_t i = 0; i < SOIL_HUMIDITY_SENSOR_NB; i++) {
                    // Read soil humidity
                    float soil_humidity[SOIL_HUMIDITY_SENSOR_NB];
                    read_soil_humidity(HUMIDITY_SOL_1, &(soil_humidity[i]));
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
                irrigation_state = IRRIGATION_IDLE;
                break;

            case IRRIGATION_WATERING:                
                /*
                valve_1 = OPEN //JC : à définir les output de valves et le "on" "off"
                vulve_2 = OPEN //JC : à définir les output de vulves et le "on" "off"
                pompe_baril = OPEN //JC : à définir les output de pompes et le "on" "off"
                */

                /* 
                    JC : à définir. En ce moment ça arrose pendant 1min puis 
                    on retourne dans idle puis si les mesures montre que c'est 
                    toujours pas assez trempe apres avoir arrosé on revient arroser 1min    
                */
                vTaskDelay(60*1000);
                /*
                pompe_baril = OPEN //JC : à définir les output de pompes et le "on" "off"
                valve_1 = CLOSE //JC : à définir les output de valves et le "on" "off"
                vulve_2 = CLOSE //JC : à définir les output de vulves et le "on" "off"
                */
                irrigation_state = IRRIGATION_IDLE;
                break;

            case IRRIGATION_RESERVOIR2BARREL:
                // pompe_reservoir = OPEN //JC : à définir les output de pompes et le "on" "off"
                vTaskDelay(60*1000); //JC : à définir. En ce moment on pompe pendant 1min. À voir s'il est précoce ou pas
                //pompe_reservoir = CLOSE //JC : à définir les output de pompes et le "on" "off"
                irrigation_state = IRRIGATION_IDLE;
                break;

            case IRRIGATION_SOAKING:
            
                //valve_1 = OPEN //JC : à définir les output de valves et le "on" "off"
                //vulve_2 = OPEN //JC : à définir les output de vulves et le "on" "off"
                //pompe_baril = OPEN //JC : à définir les output de pompes et le "on" "off"
                vTaskDelay(60*1000);
                //pompe_baril = OPEN //JC : à définir les output de pompes et le "on" "off"
                //valve_1 = CLOSE //JC : à définir les output de valves et le "on" "off"
                //vulve_2 = CLOSE //JC : à définir les output de vulves et le "on" "off"
                irrigation_state = IRRIGATION_IDLE;
                break;

            case IRRIGATION_ERROR:
            // Print error message on thingsboard
            irrigation_state = IRRIGATION_INIT;

            break;
        }
        last_irrigation_state = irrigation_state;
        context->irrigation_status = irrigation_state;
        vTaskDelay(10000);
    }
}
