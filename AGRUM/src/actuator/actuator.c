#include "actuator.h"

// The main state machine dealing with actuator
actuator_status_t actuator_sm(const measure_t measurements, const measure_state_t measure_state) {
    // actuator_state represente the state of the state machine
    static actuator_state_t actuator_state = ACTUATOR_INIT;

    // actuator_status represent the status of all
    // the actuator in the system
    static actuator_status_t actuator_status;

    switch (actuator_state) {
    case ACTUATOR_INIT:
        init_heartbeat_led();

        actuator_state = MOVING_MOTOR;
        break;

    case ACTUATOR_IDLE:
        break;

    case MOVING_MOTOR:
        rotate_pv(5,true);
        actuator_state = ACTUATOR_IDLE;
        break;

    case CALIBRATING_MOTOR:

        break;

    case PUMPING_WATER:

        break;

    case IRRIGATING:

        break;

    case ACTUATOR_ERROR:

        break;
    }

    return actuator_status;
}