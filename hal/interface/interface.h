#ifndef INTERFACE_H
#define INTERFACE_H

#include "wrap_WIFI.h"
#include "ThingsBoard.h"
#include "timing.h"

typedef enum _interface_status_t{
    INTERFACE_OK,
    INTERFACE_DISCONNECTED,
    INTERFACE_ERROR,
} interface_status_t;

/*! \brief Try to connect to the remote interface
 * 
 * \return INTERFACE_OK if init was good, INTERFACE_DISCONNECTED if wifi is disconnected
 *          and INTERFACE_ERROR if connection failed.
 */
interface_status_t connect_to_interface(void);

/*! \brief Tell if the interface is currently connected
 * 
 * \return true if connected, false if not.
 */
bool interface_is_connected(void);

#endif
