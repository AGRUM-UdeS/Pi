#include "measure.h"

// The main state machine dealing with the measurements
measure_state_t measure_sm(measure_t* measurements) {
    static measure_state_t measure_state = MEASURE_IDLE;

    switch (measure_state) {
    case MEASURE_IDLE:
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
