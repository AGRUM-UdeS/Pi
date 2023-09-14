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

void read_soil_humidity(uint8_t pin, float *value)
{
    // Read raw adc value
    uint16_t received_value;
    ADC_read_pin(ADC_IRRIGATION_ADDRESS, ADC_pin[pin], &received_value);

    // Convert adc value to relative humidity
    float adc_voltage = ADC_bits2voltage(received_value);

    *value = adc_voltage * 100.0 / 5.0;
}

/********** Energy sensors **********/
#include "ADS7828.h"
#define ADC_ENERGY_ADDRESS      (ADC_address_0)
#define ADC_PV_CURRENT_ADDRESS  (ADC_address_1)
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

static float signal2current(float signal)
{
    // Do a real conversion here
    float current = signal;
    return current;
}

float get_PV_voltage(uint8_t PV_index)
{
    // Read from ADC
    uint16_t received_value;
    ADC_read_pin(ADC_ENERGY_ADDRESS, ADC_pin[PV_index*2 + 1], &received_value);

    // Convert bits to ADC 5V ref voltage
    float adc_voltage = ADC_bits2voltage(received_value);

    // Convert voltage to PV voltage
    return adc_voltage*(100+1500)/100;
}

float get_battery_voltage(uint8_t battery_index)
{
    // Read from ADC
    uint16_t received_value;
    ADC_read_pin(ADC_address_2, ADC_pin[battery_index+2], &received_value);

    // Convert bits to ADC 5V ref voltage
    float adc_voltage = ADC_bits2voltage(received_value);

    // Convert voltage to PV voltage
    if (battery_index == 0) {
        return adc_voltage*(100+200)/100;
    } else if (battery_index == 1) {
        return adc_voltage*(100+510)/100;
    }  else {
        return -1.0;
    }
}

float get_PV_current(uint8_t PV_index)
{
    // Read from ADC
    uint16_t received_value;
    ADC_read_pin(ADC_address_2, ADC_pin[PV_index], &received_value);

    // Convert bits to ADC voltage
    float adc_voltage = ADC_bits2voltage(received_value);

    // Convert voltage to PV current
    return  (adc_voltage - VREF_PV)*(25/1.15);
}

float get_instrumentation_current(void)
{
    // Read from ADC
    uint16_t received_value;
    ADC_read_pin(ADC_ENERGY_ADDRESS, ADC_pin[INSTRU_CURRENT_ADC_PIN], &received_value);

    // Convert bits to ADC voltage
    float adc_voltage = ADC_bits2voltage(received_value);
    return  (adc_voltage - VREF_INSTRU)*(25/1.15);
}

float get_battery_current(void)
{
    // Read from ADC
    uint16_t received_value;
    ADC_read_pin(ADC_ENERGY_ADDRESS, ADC_pin[BAT_CURRENT_ADC_PIN], &received_value);

    // Convert bits to ADC voltage
    float adc_voltage = ADC_bits2voltage(received_value);
    return  (adc_voltage - VREF_BAT)*(25/1.15);
}
