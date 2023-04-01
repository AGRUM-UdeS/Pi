#include "wrap_PWM.h"

#define ONE_MHZ_DIV 125
#define FREQ_2_WRAP(x)  (1000000/x)

static uint32_t pin_wrap[28] = {0};

void init_pwm(uint8_t pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
}

void set_pwm_freq(uint8_t pin, uint32_t freq) {
    uint8_t slice = pwm_gpio_to_slice_num(pin);
        
    // Get default config
    pwm_config config = pwm_get_default_config();
    
    // Set divider, reduces counter clock to 1 MHz
    pwm_config_set_clkdiv(&config, ONE_MHZ_DIV);

    // Load the configuration into our PWM slice and start the pwm
    pwm_init(slice, &config, true);

    // Set the frequency
    pin_wrap[pin] = FREQ_2_WRAP(freq);
    pwm_set_wrap(slice, pin_wrap[pin]);
}

void enable_pwm(uint8_t pin, uint8_t duty_cycle) {
    pwm_set_gpio_level(pin, duty_cycle * pin_wrap[pin] /100);
}

void disable_pwm(uint8_t pin) {
    pwm_set_gpio_level(pin, 0);
}
