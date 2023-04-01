#include "wrap_I2C.h"

#define i2c_SDA_pin 8
#define i2c_SCL_pin 9
#define i2c_baudrate (10*1000)

#define I2C_READ_TIMEOUT    make_timeout_time_ms(10)
#define I2C_WRITE_TIMEOUT   make_timeout_time_ms(10)

void init_i2c(void) {
    i2c_init(i2c0, i2c_baudrate);
    gpio_set_function(i2c_SDA_pin, GPIO_FUNC_I2C);
    gpio_set_function(i2c_SCL_pin, GPIO_FUNC_I2C);
    gpio_pull_up(i2c_SDA_pin);
    gpio_pull_up(i2c_SCL_pin);
    printf("I2C initialized\n");
    // Add IO extender interupt pin
}

int32_t i2c0_write(uint8_t addr, const uint8_t *src, size_t len) {
    return (i2c_write_blocking_until(i2c0, addr, src, len, false, I2C_WRITE_TIMEOUT));
}

int32_t i2c0_read(uint8_t addr, uint8_t *dst, size_t len) {
    return (i2c_read_blocking_until(i2c0, addr, dst, len, false, I2C_READ_TIMEOUT));
}
