#ifndef TEST_H
#define TEST_H

#include "irrigation.h"
#include "pv_management.h"
#include "energy_management.h"
#include "PI4IOE5V9554A.h"
#include "hardware/gpio.h"
#include "sensors.h"

/*! \brief For testing and debugging purpose. Put anything you want in this
 *
 */
void develop_test(void);
void test_GPIO(int adresse, int nb_pulse);
void test_leds(int nb);
void test_GPIO_ras(int GP1, int GP2, int nb);
void test_motor_PCB(int ind_mot, int angle);
void test_Capt_temp(int adresse);

#endif