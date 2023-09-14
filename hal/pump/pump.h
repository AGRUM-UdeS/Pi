#ifndef PUMP_H
#define PUMP_H

#include "PI4IOE5V9554A.h"

typedef enum pump_state_t{
  PUMP_ON,
  PUMP_OFF,
  PUMP_UNKNOWN
} pump_state_t; 

/*! \brief turn off every pumps
 */
void disable_all_pump(void);

/*! \brief Start one pump
 *
 * \param pump_id io pin id of the pump
 * 
 * \return PUMP_ON if success, PUMP_UNKNOWN if failed
 */
pump_state_t enable_pump(uint8_t pump_id);

/*! \brief Disable one pump
 *
 * \param pump_id io pin id of the pump
 * 
 * \return PUMP_OFF if success, PUMP_UNKNOWN if failed
 */
pump_state_t disable_pump(uint8_t pump_id);

#endif