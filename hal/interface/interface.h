#ifndef INTERFACE_H
#define INTERFACE_H

#include "wrap_WIFI.h"
#include "ThingsBoard.h"
#include "timing.h"

#define PI_STATUS_TOPIC         ("Pi status")
#define PI_STATUS_CONNECTED     (0.1)
#define PI_STATUS_PING          (0.2)

typedef enum _interface_status_t{
    INTERFACE_CONNECTED,
    INTERFACE_CONNECTING,
    INTERFACE_DISCONNECTED,
    INTERFACE_ERROR,
} interface_status_t;

/*! \brief Try to connect to the remote interface
 * 
 * \return INTERFACE_CONNECTED if init was good, INTERFACE_DISCONNECTED if wifi is disconnected
 *          and INTERFACE_ERROR if connection failed.
 */
interface_status_t connect_to_interface(void);

/*! \brief Tell if the interface is currently connected
 * 
 * \return true if connected, false if not.
 */
bool interface_is_connected(void);

/*! \brief State-machine managing the interface connection
 * 
 * \return The state in which the state-machine was.
 */
interface_status_t interface_sm(void);

#endif
