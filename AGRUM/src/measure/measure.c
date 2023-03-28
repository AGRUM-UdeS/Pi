#include "measure.h"

// The main state machine dealing with the measurements
measure_state_t measure_sm(measure_t* measurements) {
    static measure_state_t measure_state = MEASURE_IDLE;
    uint16_t value;

    switch (measure_state) {
    case MEASURE_IDLE:
        // ADC_read_pin(ADC_address_0, ADC_read_pin_4, &value);
        // printf("Value read from ADC: %d. Or %.2f V\n", value, (float)(value*5.0/4096.0));
        measure_state = MESURE_DONE;
        // wait for time to measure
        break;

    case MEASURING:
        // Take all the measurements

        break;

    case MESURE_DONE:
        // ??

        break;

    case MEASURE_ERROR:
        // Manage errors

        break;
    }


    return measure_state;
}
