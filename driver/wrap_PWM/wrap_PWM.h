#ifndef WRAP_PWM_H
#define WRAP_PWM_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

/*! \brief Set the pin as a pwm pin
 *
 * \param pin GPIO pin number to set as pwm
 */
void init_pwm(uint8_t pin);

/*! \brief Set the pwm frequency
 *
 * \note PWM pin must be init before
 * \param pin GPIO pin number
 * \param freq PWM frequency in Hz
 */
void set_pwm_freq(uint8_t pin, uint32_t freq);

/*! \brief Enable the PWM output with set duty cycle
 *
 * \note PWM pin must be init before
 * \param pin GPIO pin number
 * \param duty_cycle duty cycle in pourcentage (0-100)
 */
void enable_pwm(uint8_t pin, uint8_t duty_cycle);

/*! \brief Disable the PWM output, set duty cycle to 0
 *
 * \note PWM pin must be init before
 * \param pin GPIO pin number
 */
void disable_pwm(uint8_t pin);

#endif