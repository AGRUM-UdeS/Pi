#include "I2C_wrapper.h"

#define i2c_SDA_pin 8
#define i2c_SCL_pin 9
#define i2c_baudrate (100*1000)

void init_i2c(void) {
    i2c_init(i2c0, i2c_baudrate);
    gpio_set_function(i2c_SDA_pin, GPIO_FUNC_I2C);
    gpio_set_function(i2c_SCL_pin, GPIO_FUNC_I2C);
    gpio_pull_up(i2c_SDA_pin);
    gpio_pull_up(i2c_SCL_pin);
    printf("I2C initialized\n");
    // Add IO extender interupt pin
}
