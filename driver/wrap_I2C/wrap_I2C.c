#include "wrap_I2C.h"

#define i2c0_SDA_pin 8
#define i2c0_SCL_pin 9
#define i2c1_SDA_pin 6
#define i2c1_SCL_pin 7
#define i2c_baudrate (10*1000)

#define I2C_READ_TIMEOUT    make_timeout_time_ms(10)
#define I2C_WRITE_TIMEOUT   make_timeout_time_ms(10)

void init_i2c(void) {
    // Init i2c0
    i2c_init(i2c0, i2c_baudrate);
    gpio_set_function(i2c0_SDA_pin, GPIO_FUNC_I2C);
    gpio_set_function(i2c0_SCL_pin, GPIO_FUNC_I2C);
    // gpio_pull_up(i2c0_SDA_pin);
    // gpio_pull_up(i2c0_SCL_pin);

    // Init i2c1
    i2c_init(i2c1, i2c_baudrate);
    gpio_set_function(i2c1_SDA_pin, GPIO_FUNC_I2C);
    gpio_set_function(i2c1_SCL_pin, GPIO_FUNC_I2C);
    // gpio_pull_up(i2c1_SDA_pin);
    // gpio_pull_up(i2c1_SCL_pin);
    printf("I2C initialized\n");
    // Add IO extender interupt pin
}

/*! \brief Write with i2c0 peripheral
 *
 * \note i2c0 needs to be init before
 *
 * \param addr Address of the device to write to
 * \param src Pointer to the buffer that contains bytes to send
 * \param len Number of bytes to send
 * 
 * \return Number of bytes written, or 
 * PICO_ERROR_GENERIC if address not acknowledged, no device present, or 
 * PICO_ERROR_TIMEOUT if a timeout occurred.
 */
int32_t i2c0_write(uint8_t addr, const uint8_t *src, size_t len) {
    taskENTER_CRITICAL();
    int rv = (i2c_write_blocking_until(i2c0, addr, src, len, false, I2C_WRITE_TIMEOUT));
    taskEXIT_CRITICAL();
    return rv;
}

/*! \brief Read with i2c0 peripheral
 *
 * \note i2c0 needs to be init before
 *
 * \param addr Address of the device to read from
 * \param dst Pointer to store received bytes
 * \param len Number of bytes to receive
 * 
 * \return Number of bytes written, or 
 * PICO_ERROR_GENERIC if address not acknowledged, no device present, or 
 * PICO_ERROR_TIMEOUT if a timeout occurred.
 */
int32_t i2c0_read(uint8_t addr, uint8_t *dst, size_t len) {
    taskENTER_CRITICAL();
    int rv = (i2c_read_blocking_until(i2c0, addr, dst, len, false, I2C_READ_TIMEOUT));
    taskEXIT_CRITICAL();
    return rv;
}

/*! \brief Write with i2c1 peripheral
 *
 * \note i2c1 needs to be init before
 *
 * \param addr Address of the device to write to
 * \param src Pointer to the buffer that contains bytes to send
 * \param len Number of bytes to send
 * 
 * \return Number of bytes written, or 
 * PICO_ERROR_GENERIC if address not acknowledged, no device present, or 
 * PICO_ERROR_TIMEOUT if a timeout occurred.
 */
int32_t i2c1_write(uint8_t addr, const uint8_t *src, size_t len) {
    taskENTER_CRITICAL();
    int rv = (i2c_write_blocking_until(i2c1, addr, src, len, false, I2C_WRITE_TIMEOUT));
    taskEXIT_CRITICAL();
    return rv;
}

/*! \brief Read with i2c1 peripheral
 *
 * \note i2c1 needs to be init before
 *
 * \param addr Address of the device to read from
 * \param dst Pointer to store received bytes
 * \param len Number of bytes to receive
 * 
 * \return Number of bytes written, or 
 * PICO_ERROR_GENERIC if address not acknowledged, no device present, or 
 * PICO_ERROR_TIMEOUT if a timeout occurred.
 */
int32_t i2c1_read(uint8_t addr, uint8_t *dst, size_t len) {
    taskENTER_CRITICAL();
    int rv = (i2c_read_blocking_until(i2c1, addr, dst, len, false, I2C_READ_TIMEOUT));
    taskEXIT_CRITICAL();
    return rv;
}
