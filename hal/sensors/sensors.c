#include "sensors.h"

/********** Irrigation sensors **********/
#define ADC_IRRIGATION_ADDRESS  (ADC_address_1)
#define IO_IRRIGATION_ADDRESS   (IO_address_0)
#define BARREL_WATER_LEVEL_PIN  (0)
#define BAC_WATER_LEVEL_PIN     (1)

uint8_t enviro_sensor_location[ENVIRO_SENSOR_NB] = {
    UNDER_PV,
    BESIDE_PV
};

void init_water_level_sensors(void)
{
    // Do nothing
    // IO pin are set as input by default
}

bool bac_is_full(void)
{
    uint8_t value;
    IO_status_t status = IO_read_pin(IO_IRRIGATION_ADDRESS, BAC_WATER_LEVEL_PIN, &value);
    if (status == IO_ok && !value) {
        return true;
    }
    return false;
}

bool barrel_is_empty(void)
{
    uint8_t value;
    IO_status_t status = IO_read_pin(IO_IRRIGATION_ADDRESS, BARREL_WATER_LEVEL_PIN, &value);
    if (status == IO_ok && value) {
        return true;
    }
    return false;
}

SHT_measure_t read_temp_humidity(uint8_t sensor)
{
    SHT_measure_t meas;

    switch(sensor) {
    case UNDER_PV:
        if (SHT3_read_temp_humidity(&(meas.temp), &(meas.humidity), 0) != SHT_ok) {
            meas.meas_ok = false;
        } else {
            meas.meas_ok = true;
        }
        break;

    case BESIDE_PV:
        if (SHT3_read_temp_humidity(&(meas.temp), &(meas.humidity), 1) != SHT_ok) {
            meas.meas_ok = false;
        } else {
            meas.meas_ok = true;
        }

        break;

    default:
        meas.meas_ok = false;

        break;
    }

    return meas;
}

void read_soil_humidity(uint8_t pin, float *value)
{
    // Read raw adc value
    uint16_t received_value;
    ADC_read_pin(ADC_IRRIGATION_ADDRESS, pin, &received_value);

    // Convert adc value to relative humidity
    float adc_voltage = ADC_bits2voltage(received_value);

    *value = adc_voltage * 100.0 / 5.0;
}

/********** Energy sensors **********/
#include "ADS7828.h"
#define ADC_ENERGY_ADDRESS      (ADC_address_0)
#define ADC_PV_CURRENT_ADDRESS  (ADC_address_1)
#define ADC_EXTERN_ADDRESS      (ADC_address_2)
// To adjust with real set-up
#define PV_VOLTAGE1_PIN 0
#define PV_VOLTAGE2_PIN 1
#define PV_VOLTAGE3_PIN 2
#define PV_VOLTAGE4_PIN 3
#define PV_CURRENT1_PIN 4
#define PV_CURRENT2_PIN 5
#define PV_CURRENT3_PIN 6
#define PV_CURRENT4_PIN 7

#define BAT_CURRENT_ADC_PIN     6
#define INSTRU_CURRENT_ADC_PIN  7

#define VREF_PV         (1.615)
#define VREF_INSTRU     (2.51)
#define VREF_BAT        (2.51)

#define NB_MEASUREMENT_AVG  (5)

#define INDEX_TO_PV_VOLTAGE(x)  (x)
#define INDEX_TO_PV_CURRENT(x)  (x+2)

float get_PV_voltage(uint8_t PV_index)
{
    // Read from ADC
    uint16_t received_value[NB_MEASUREMENT_AVG] = {0};
    float adc_voltage[NB_MEASUREMENT_AVG] = {0};
    float sum = 0;

    for (size_t i = 0; i < NB_MEASUREMENT_AVG; i++) {
        ADC_read_pin(ADC_EXTERN_ADDRESS, INDEX_TO_PV_VOLTAGE(PV_index), &(received_value[i]));

        // Convert bits to ADC 5V ref voltage
        adc_voltage[i] = ADC_bits2voltage(received_value[i]);
        sum += adc_voltage[i];
    }

    // Convert voltage to PV voltage
    return sum/(float)(NB_MEASUREMENT_AVG)*(75.0+1500.0)/75.0;
}

float get_battery_voltage(uint8_t battery_index)
{
    // Read from ADC
    uint16_t received_value[NB_MEASUREMENT_AVG];
    float adc_voltage[NB_MEASUREMENT_AVG];
    float sum = 0;

    for (size_t i = 0; i < NB_MEASUREMENT_AVG; i++) {
        ADC_read_pin(ADC_ENERGY_ADDRESS, battery_index, &(received_value[i]));

        // Convert bits to ADC 5V ref voltage
        adc_voltage[i] = ADC_bits2voltage(received_value[i]);
        sum += adc_voltage[i];
    }
    float avg = sum/(float)NB_MEASUREMENT_AVG;

    // Convert voltage to PV voltage
    if (battery_index == 1) {
        return avg*(100.0+200)/100.0;
    } else if (battery_index == 3) {
        return avg*(100.0+510)/100.0;
    }  else {
        return -1.0;
    }
}

float get_PV_current(uint8_t PV_index)
{
    // Read from ADC
    uint16_t received_value[NB_MEASUREMENT_AVG];
    float adc_voltage[NB_MEASUREMENT_AVG];
    float sum = 0;

    for (size_t i = 0; i < NB_MEASUREMENT_AVG; i++) {
        ADC_read_pin(ADC_EXTERN_ADDRESS, INDEX_TO_PV_CURRENT(PV_index), &(received_value[i]));

        // Convert bits to ADC 5V ref voltage
        adc_voltage[i] = ADC_bits2voltage(received_value[i]);
        sum += adc_voltage[i];
    }
    float avg = sum/NB_MEASUREMENT_AVG;

    // Convert voltage to PV current
    return  (avg - VREF_PV)*(25.0/1.15);
}

float get_instrumentation_current(void)
{
    // Read from ADC
    uint16_t received_value[NB_MEASUREMENT_AVG];
    float adc_voltage[NB_MEASUREMENT_AVG];
    float sum = 0;

    for (size_t i = 0; i < NB_MEASUREMENT_AVG; i++) {
        ADC_read_pin(ADC_ENERGY_ADDRESS, INSTRU_CURRENT_ADC_PIN, &(received_value[i]));

        // Convert bits to ADC 5V ref voltage
        adc_voltage[i] = ADC_bits2voltage(received_value[i]);
        sum += adc_voltage[i];
    }
    float avg = sum/NB_MEASUREMENT_AVG;

    return  (avg - VREF_INSTRU)*(25/1.15);
}

float get_battery_current(void)
{
    // Read from ADC
    uint16_t received_value[NB_MEASUREMENT_AVG];
    float adc_voltage[NB_MEASUREMENT_AVG];
    float sum = 0;

    for (size_t i = 0; i < NB_MEASUREMENT_AVG; i++) {
        ADC_read_pin(ADC_ENERGY_ADDRESS, BAT_CURRENT_ADC_PIN, &(received_value[i]));

        // Convert bits to ADC 5V ref voltage
        adc_voltage[i] = ADC_bits2voltage(received_value[i]);
        sum += adc_voltage[i];
    }
    float avg = sum/NB_MEASUREMENT_AVG;

    return  (avg - VREF_BAT)*(25/1.15);
}
