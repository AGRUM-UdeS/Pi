#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "wrap_I2C.h"
#include "wrap_WATCHDOG.h"
#include "interface.h"

/*! \brief Blocking delay to let the developper open a terminal
 * 
 */
void usb_delay(uint8_t delay_s);

/*! \brief Init peripherals pinout and configurations
 * 
 */
void init_peripherals(void);

/*! \brief Some house keeping task. (Watchdog, interface ping, etc)
 * 
 */
void house_keeping(void);

#endif
