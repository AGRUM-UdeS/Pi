#include "PI4IOE5V9554A.h"

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

static IO_status_t IO_config_reg(uint8_t address, uint8_t* byte)
{
    // Send the read command
    uint8_t msg = IO_PIN_DIR;
    int nb = i2c0_write(address, &msg, sizeof(msg));
    // Read the pins state from the IO expander
    nb = i2c0_read(address, byte, (size_t)1);

    return return_IO_status(nb);
}

static IO_status_t IO_read_output_reg(uint8_t address, uint8_t* received_data){
    // Send the read command
    uint8_t byte = IO_WRITE;
    int nb = i2c0_write(address, &byte, sizeof(byte));
    // Read the pins state from the IO expander
    nb = i2c0_read(address, received_data, (size_t)1);

    return return_IO_status(nb);
}

static IO_status_t IO_read_port(uint8_t address, uint8_t* received_data){
    // Send the read command
    uint8_t byte = IO_READ;
    int nb = i2c0_write(address, &byte, sizeof(byte));
    // Read the pins state from the IO expander
    nb = i2c0_read(address, received_data, (size_t)1);

    return return_IO_status(nb);
}

static IO_status_t IO_write_port(uint8_t address, uint8_t port){
    char byte[] = {IO_WRITE, port};
    int nb = i2c0_write(address, byte, sizeof(byte));

    return return_IO_status(nb);
}

IO_status_t IO_set_as_output(uint8_t address, uint8_t pin){
    if (pin >= 8)
        return IO_error;

    uint8_t byte = 0;
    uint8_t mask = (1 << pin);
    // Read current value of the port
    IO_status_t status = IO_config_reg(address, &byte);

    if (status == IO_ok) {
        // Choose the pin to set
        byte &= ~mask;
    } else {
        return status;
    }

    char msg[] = {IO_PIN_DIR, byte};
    int nb = i2c0_write(address, msg, sizeof(msg));

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
    IO_status_t status = IO_read_output_reg(address, &byte);

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
    status = IO_read_output_reg(address, &byte);
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
    IO_status_t status = IO_read_output_reg(address, &byte);

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
    status = IO_read_output_reg(address, &byte);
    if (byte != rec_byte || status == IO_ok) {
        return IO_error;
    }

    return status;
}
