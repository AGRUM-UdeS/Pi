#ifndef MOTOR_H
#define MOTOR_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "PI4IOE5V9554A.h"
#include "wrap_PWM.h"

typedef enum motor_state_t {
  MOTOR_ON_CLOCKWISE,
  MOTOR_ON_COUNTERCLOCKWISE,
  MOTOR_OFF,
  MOTOR_CALIBRATING,
  MOTOR_UNKNOWN
} motor_state_t;

/*! \brief Init pin used for the motor drive. (Pul, dir, en)
 */
void init_motor(void);

/*! \brief Rotate the PV by a set amount of angle in degree,
 * clockise or counterclockwise.
 *
 * \note Counter clockwise follow the sun.
 *
 * \param angle Number of degree to move the panels
 * \param port Direction to move the panels
 */
void rotate_pv(uint16_t angle, bool clockwise);

void rotate_single_pv(int ind_motor, uint16_t angle, bool clockwise);

static int64_t stop_rotation_mot1(__unused alarm_id_t id, __unused void *user_data);
static int64_t stop_rotation_mot2(__unused alarm_id_t id, __unused void *user_data);
static int64_t stop_rotation_mot3(__unused alarm_id_t id, __unused void *user_data);
static int64_t stop_rotation_mot4(__unused alarm_id_t id, __unused void *user_data);

#endif