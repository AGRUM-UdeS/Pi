#include "sensors.h"

/********** Irrigation sensors **********/
#define ADC_IRRIGATION_ADDRESS  (ADC_address_1)
#define IO_IRRIGATION_ADDRESS   (IO_address_0)
#define BARREL_WATER_LEVEL_PIN  (0)
#define BAC_WATER_LEVEL_PIN     (1)

uint8_t enviro_sensor_location[ENVIRO_SENSOR_NB] = {
    UNDER_PV_0,
    UNDER_PV_1,
    BESIDE_PV_0,
    BESIDE_PV_1
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
    case UNDER_PV_0:
        if (SHT3_read_temp_humidity(&(meas.temp), &(meas.humidity), 0) != SHT_ok) {
            meas.meas_ok = false;
        } else {
            meas.meas_ok = true;
        }
        break;

    case UNDER_PV_1:
        meas.meas_ok = false;

        break;

    case BESIDE_PV_0:
        if (SHT3_read_temp_humidity(&(meas.temp), &(meas.humidity), 1) != SHT_ok) {
            meas.meas_ok = false;
        } else {
            meas.meas_ok = true;
        }

        break;

    case BESIDE_PV_1:
        meas.meas_ok = false;

        break;

    default:
        meas.meas_ok = false;

        break;
    }

    return meas;
}

void read_soil_humidity(uint8_t adc_pin, float *value)
{
    // Read raw adc value
    uint16_t raw_adc_value;
    ADC_read_pin(ADC_IRRIGATION_ADDRESS, adc_pin, &raw_adc_value);

    // Convert adc value to relative humidity
    *value = ((float)raw_adc_value * (100.0) / 4096.0);
}

/********** Energy sensors **********/
#include "ADS7828.h"
#define ADC_ENERGY_ADDRESS   (ADC_address_0)
// To adjust with real set-up
#define PV_VOLTAGE1_PIN 0
#define PV_VOLTAGE2_PIN 1
#define PV_VOLTAGE3_PIN 2
#define PV_VOLTAGE4_PIN 3
#define PV_CURRENT1_PIN 4
#define PV_CURRENT2_PIN 5
#define PV_CURRENT3_PIN 6
#define PV_CURRENT4_PIN 7

static float signal2current(float signal)
{
    // Do a real conversion here
    float current = signal;
    return current;
}

static float signal2voltage(float signal)
{
    // Do a real conversion here
    float voltage = signal;
    return voltage;
}

float get_PV_voltage(uint8_t PV_index)
{
    // Read from ADC
    uint16_t received_value;
    ADC_read_pin(ADC_ENERGY_ADDRESS, ADC_pin[PV_index], &received_value);

    // Convert bits to ADC voltage
    float adc_voltage = ADC_bits2voltage(received_value);

    // Convert voltage to PV voltage
    return signal2voltage(adc_voltage);
}

float get_PV_current(uint8_t PV_index)
{
    // Read from ADC
    uint16_t received_value;
    ADC_read_pin(ADC_ENERGY_ADDRESS, ADC_pin[(PV_index + 4)], &received_value);

    // Convert bits to ADC voltage
    float adc_voltage = ADC_bits2voltage(received_value);

    // Convert voltage to PV current
    return signal2current(adc_voltage);
}
