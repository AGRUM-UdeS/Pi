#ifndef PI4IOE5V9554A_H
#define PI4IOE5V9554A_H

#include "pico/stdlib.h"
#include "wrap_I2C.h"
#include <stdio.h>

#define IO_address_0      (0x38)
#define IO_address_1      (0x39)
#define IO_address_2      (0x3A)
#define IO_address_3      (0x3B)
#define IO_address_4      (0x3C)
#define IO_address_5      (0x3D)
#define IO_address_6      (0x3E)
#define IO_address_7      (0x3F)

#define IO_READ           (0x0)
#define IO_WRITE          (0x1)
#define IO_POLARITY_INV   (0x2)
#define IO_PIN_DIR        (0x3)

typedef enum _IO_status_t{
  IO_ok,
  IO_timeout,
  IO_error
} IO_status_t;

/*! \brief Set pin bit set as output
 *
 * \note Pin are input by default
 *
 * \param address address of the i2c chip
 * \param port pin to set as output. ex: 0x13 (0b00010011) will set pin 
 * 4, 1 and 0 as output
 * \return IO_ok if everything good, IO_timeout if timeout and IO_error if other errors
 */
IO_status_t IO_set_as_output(uint8_t address, uint8_t port);

/*! \brief Read the input register
 *
 * \note Pin are read even if they are set as output
 *
 * \param address address of the i2c chip
 * \param received_data Used to store all 8 pins states
 * \return IO_ok if everything good, IO_timeout if timeout and IO_error if other errors
 */
IO_status_t IO_read_port(uint8_t address, uint8_t* received_data);

/*! \brief Read one pin state
 *
 * \note Pin are read even if it is set as output
 *
 * \param address address of the i2c chip
 * \param pin Pin to read (0-7)
 * \param value Used to store the value of the pin
 * \return IO_ok if everything good, IO_timeout if timeout and IO_error if other errors
 */
IO_status_t IO_read_pin(uint8_t address, uint8_t pin, uint8_t* value);

/*! \brief Write to the output register
 *
 * \note Pins first need to be set as output
 *
 * \param address address of the i2c chip
 * \param port Value to write to the output register
 * \return IO_ok if everything good, IO_timeout if timeout and IO_error if other errors
 */
IO_status_t IO_write_port(uint8_t address, uint8_t port);

/*! \brief Set one pin to high
 *
 * \note Pins first need to be set as output
 *
 * \param address address of the i2c chip
 * \param pin Pin to set to high (0-7)
 * \return IO_ok if everything good, IO_timeout if timeout and IO_error if other errors
 */
IO_status_t IO_set_pin(uint8_t address, uint8_t pin);

/*! \brief Set one pin to low
 *
 * \note Pins first need to be set as output
 *
 * \param address address of the i2c chip
 * \param pin Pin to set to low (0-7)
 * \return IO_ok if everything good, IO_timeout if timeout and IO_error if other errors
 */
IO_status_t IO_clear_pin(uint8_t address, uint8_t pin);

#endif
