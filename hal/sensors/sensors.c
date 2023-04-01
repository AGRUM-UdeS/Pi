#include "sensors.h"

/********** Irrigation sensors **********/
#define IO_IRRIGATION_ADDRESS   (IO_address_0)
#define BARREL_WATER_LEVEL_PIN  (0)
#define BIN_WATER_LEVEL_PIN     (1)

void init_water_level_sensors(void)
{
    // Do nothing
    // IO pin are set as input by default
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
