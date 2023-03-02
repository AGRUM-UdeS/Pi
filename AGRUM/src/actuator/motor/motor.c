#include "motor.h"

#define pul_1_pin 12
#define pul_2_pin 13
#define pul_3_pin 14
#define pul_4_pin 15

void init_pwm(void) {
    // Set pwm pins
    gpio_set_function(pul_1_pin, GPIO_FUNC_PWM);
    gpio_set_function(pul_2_pin, GPIO_FUNC_PWM);
    gpio_set_function(pul_3_pin, GPIO_FUNC_PWM);
    gpio_set_function(pul_4_pin, GPIO_FUNC_PWM);

    // Get which pin is which slice
    uint8_t slice_1 = pwm_gpio_to_slice_num(pul_1_pin);
    uint8_t slice_2 = pwm_gpio_to_slice_num(pul_2_pin);
    uint8_t slice_3 = pwm_gpio_to_slice_num(pul_3_pin);
    uint8_t slice_4 = pwm_gpio_to_slice_num(pul_4_pin);

    // Get default config
    pwm_config config = pwm_get_default_config();
    // Set divider, reduces counter clock to sysclock/this value
    // pwm_config_set_clkdiv(&config, 4.f);

    // Load the configuration into our PWM slice and start the pwm
    pwm_init(slice_1, &config, true);
    pwm_init(slice_1, &config, true);
    pwm_init(slice_1, &config, true);
    pwm_init(slice_1, &config, true);

    // Set 256 as 100% duty cycle
    pwm_set_wrap(slice_1, 256);
    pwm_set_wrap(slice_2, 256);
    pwm_set_wrap(slice_3, 256);
    pwm_set_wrap(slice_4, 256);

}

void pwm_test(void) {
    static uint32_t DC = 1;
    pwm_set_gpio_level(pul_1_pin, DC);
    DC = DC*2;
    if (DC > 256)
        DC = 1;
}

