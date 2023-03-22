#include "measure.h"

// The main state machine dealing with the measurements
measure_state_t measure_sm(measure_t* measurements) {
    static measure_state_t measure_state = MEASURE_IDLE;
    uint8_t buf[1];

    switch (measure_state) {
    case MEASURE_IDLE:
        IO_read_port(IO_address_0, buf);
        printf("Received buf: 0x%x\n", buf[0]);
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
