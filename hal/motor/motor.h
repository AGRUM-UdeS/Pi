#ifndef MOTOR_H
#define MOTOR_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "PI4IOE5V9554A.h"
#include "wrap_PWM.h"

#define COUNTERCLOCKWISE    false 
#define CLOCKWISE           true

typedef enum motor_state_t {
  MOTOR_OK,
  MOTOR_ERROR,
  MOTOR_ON_CLOCKWISE,
  MOTOR_ON_COUNTERCLOCKWISE,
  MOTOR_OFF,
  MOTOR_CALIBRATING,
  MOTOR_UNKNOWN
} motor_state_t;

/*! \brief Init pin used for the motor drive. (Pul, dir, en)
 *  \return MOTOR_OK if init was good, MOTOR_ERROR if not
 */
motor_state_t init_motor(void);

/*! \brief Rotate all the PV by a set amount of angle in degree,
 * clockise or counterclockwise.
 *
 * \note Counter clockwise follow the sun.
 *
 * \param angle Number of degree to move the panels
 * \param clockwise Direction to move the panels
 * 
 * \return MOTOR_OK if init was good, MOTOR_ERROR if not
 */
motor_state_t rotate_all_pv(uint16_t angle, bool clockwise);

/*! \brief Rotate only one PV by a set amount of angle in degree,
 * clockise or counterclockwise.
 *
 * \note Counter clockwise follow the sun.
 *
 * \param ind_motor Index of the motor to move 
 * \param angle Number of degree to move the panels
 * \param clockwise Direction to move the panels
 * 
 * \return MOTOR_OK if init was good, MOTOR_ERROR if not
 */
motor_state_t rotate_single_pv(uint8_t ind_motor, uint16_t angle, bool clockwise);

/*! \brief Stop single motor rotation
 *
 * \param ind_motor Index of the motor to move
 * 
 * \return MOTOR_OK if init was good, MOTOR_ERROR if not
 */
motor_state_t stop_single_pv(uint8_t ind_motor);

/*! \brief Tell if every motor is moving
 *
 * \return true if every motor is moving, false if not
 */
bool all_motor_moving(void);
 
/*! \brief Stop single motor rotation
 *
 * \param switch_nb Number of limit switch used
 * \return true if a limit switch is touched, false if not
 */
bool limit_switch_touched(uint8_t lm_pin_value[], uint16_t switch_nb);

/*! \brief Convert timing to angle
 */
uint32_t ms2angle(uint32_t time);
#endif