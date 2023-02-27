#include "PI4IOE5V9554.h"

const absolute_time_t ABSOLUTE_TIME_INITIALIZED_VAR(IO_i2c_timeout, 10000); // 10 ms

IO_status_t return_IO_status(int value){
    IO_status_t rv;

    if (rv == PICO_ERROR_GENERIC) {
        rv = IO_error;
    } else if (rv == PICO_ERROR_TIMEOUT) {
        rv = IO_timeout;
    } else {
        rv = IO_ok;
    }

    return rv;
}

IO_status_t IO_read(uint address, uint8_t* received_data){
    IO_status_t status = IO_ok;

    int nb = i2c_read_blocking_until(i2c1, IO_address_0, received_data, (size_t)1, false, IO_i2c_timeout);

    return return_IO_status(nb);
}
