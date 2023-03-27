#include "PI4IOE5V9554.h"

#define IO_i2c_timeout make_timeout_time_ms(10)

static IO_status_t return_IO_status(int value){
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

IO_status_t IO_set_as_output(uint8_t address, uint8_t port){
    char byte[] = {IO_PIN_DIR, ~(1 << port)};
    int nb = i2c_write_blocking_until(i2c0, address, byte, sizeof(byte), false, IO_i2c_timeout);
    return return_IO_status(nb);
}

IO_status_t IO_read_port(uint8_t address, uint8_t* received_data){
    // Send the read command
    int nb = i2c_write_blocking_until(i2c0, address, IO_READ, (size_t)1, false, IO_i2c_timeout);
    // Read the pins state from the IO expander
    nb = i2c_read_blocking_until(i2c0, address, received_data, (size_t)1, false, IO_i2c_timeout);

    return return_IO_status(nb);
}

IO_status_t IO_write_port(uint8_t address, uint8_t port){
    char byte[] = {IO_WRITE, port};
    int nb = i2c_write_blocking_until(i2c0, address, byte, sizeof(byte), false, IO_i2c_timeout);

    return return_IO_status(nb);
}

IO_status_t IO_read_pin(uint8_t address, uint8_t pin, uint8_t* value){
    if (pin >= 8)
        return IO_error;

    uint8_t mask = (1 << pin);
    uint8_t byte = 0;

    IO_status_t status = IO_read_port(address, &byte);

    // Value is left unchanged if read fails
    if (status == IO_ok)
        *value = (byte & mask) >> pin;

    return status;
}

IO_status_t IO_set_pin(uint8_t address, uint8_t pin){
    if (pin >= 8)
        return IO_error;

    uint8_t byte = 0;
    uint8_t mask = (1 << pin);
    // Read current value of the port
    IO_status_t status = IO_read_port(address, &byte);

    if (status == IO_ok) {
        // Choose the pin to set
        byte |= mask;
    } else {
        return status;
    }
    // Set the pin to high, leaving other unchanged
    status = IO_write_port(address, byte);

    if (status != IO_ok) {
        return IO_error;
    }
    // Make sure pin has changed
    uint8_t rec_byte = byte;
    status = IO_read_port(address, &byte);
    if (byte != rec_byte || status == IO_ok) {
        return IO_error;
    }

    return status;
}

IO_status_t IO_clear_pin(uint8_t address, uint8_t pin){
    if (pin >= 8)
        return IO_error;

    uint8_t byte = 0;
    uint8_t mask = (1 << pin);
    // Read current value of the port
    IO_status_t status = IO_read_port(address, &byte);

    if (status == IO_ok) {
        // Choose the pin to set
        byte &= ~mask;
    } else {
        return status;
    }
    // Set the pin to high, leaving other unchanged
    status = IO_write_port(address, byte);

    if (status != IO_ok) {
        return IO_error;
    }
    // Make sure pin has changed
    uint8_t rec_byte = byte;
    status = IO_read_port(address, &byte);
    if (byte != rec_byte || status == IO_ok) {
        return IO_error;
    }

    return status;
}
