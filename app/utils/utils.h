#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "wrap_I2C.h"
#include "wrap_WATCHDOG.h"
#include "interface.h"
#include "led.h"
#include "irrigation.h"
#include "energy_management.h"

// TODO: Add more states.
typedef enum _system_status_t{
    SYSTEM_IDLE,
    SYSTEM_MEASURING,
    SYSTEM_PV_ROTATION,
    SYSTEM_IRRIGATING,
    SYSTEM_POWER_INJECTION,
    SYSTEM_PV_CALIBRATION,
    SYSTEM_WATER_PUMPING,
    SYSTEM_ERROR
} system_status_t;

/*! \brief Blocking delay to let the developper open a terminal
 * 
 */
void usb_delay(uint8_t delay_s);

/*! \brief Init every hardware not wifi dependent
 * 
 */
void init_hardware(void *ptr);

/*! \brief Init timer and rtc
 * 
 */
void init_timing(void);

/*! \brief Some house keeping task. (Watchdog, interface ping, etc)
 * 
 */
void house_keeping(void *pvParameters);

void clear_pv_calib_flag (void);
bool morning_pv_calibration(void);
void clear_weather_flag (void);
bool morning_weather(void);
void clear_irrigation_flag (void);
bool morning_irrigation(void);

unsigned int countSetBits(unsigned int n);

#endif
