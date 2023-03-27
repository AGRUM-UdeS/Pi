#include "measure.h"

// The main state machine dealing with the measurements
measure_state_t measure_sm(measure_t* measurements) {
    static measure_state_t measure_state = MEASURE_IDLE;
    uint8_t value= 34;

    switch (measure_state) {
    case MEASURE_IDLE:
        IO_set_as_output(IO_address_0, 0);
        IO_set_pin(IO_address_0, 0);
        IO_clear_pin(IO_address_0, 0);
        printf("Received value: %x\n", value);
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
