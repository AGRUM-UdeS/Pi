#ifndef INTERFACE_H
#define INTERFACE_H

#include "wrap_WIFI.h"
#include "ThingsBoard.h"
#include "timing.h"

#define PI_STATUS_TOPIC         ("Pi status")
#define PI_STATUS_CONNECTED     (0)
#define PI_STATUS_PING          (1)

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

/*! \brief Publish a value to a topic on the interface
 * 
 * \param topic Topic string
 * \param value Value to publish to the topi
 * \return true if published, false if not.
 */
bool interface_publish(unsigned char *topic, float value);

/*! \brief State-machine managing the interface connection
 * 
 * \return The state in which the state-machine was.
 */
interface_status_t interface_sm(void);

#endif
