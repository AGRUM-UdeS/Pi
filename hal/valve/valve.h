#ifndef VALVE_H
#define VALVE_H

#include "PI4IOE5V9554A.h"

typedef enum {
  VALVE_OPEN,
  VALVE_CLOSED,
  VALVE_UNKNOWN
} valve_state_t;

/*! \brief Close every valves
 */
void close_all_valve(void);

/*! \brief Open one valve
 *
 * \param valve_id io pin id of the valve
 * 
 * \return VALVE_OPEN if success, VALVE_UNKNOWN if failed
 */
valve_state_t open_valve(uint8_t valve_id);

/*! \brief Close one valve
 *
 * \param valve_id io pin id of the valve
 * 
 * \return VALVE_CLOSED if success, VALVE_UNKNOWN if failed
 */
valve_state_t close_valve(uint8_t valve_id);

#endif