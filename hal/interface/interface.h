#ifndef INTERFACE_H
#define INTERFACE_H

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "context.h"
#include "wrap_WIFI.h"
#include "ThingsBoard.h"
#include "timing.h"
#include "sensors.h"

#define PI_STATUS_TOPIC         ("Pi status")
#define SYSTEM_STATUS_TOPIC     ("System state")
#define ENERGY_STATUS_TOPIC     ("Energy state")
#define PI_STATUS_CONNECTED     (0)
#define PI_STATUS_PING          (1)
#define MAX_TOPIC_LEN           (64)

typedef struct _mqtt_message {
    unsigned char topic[MAX_TOPIC_LEN];
    float value;
} mqtt_message_t;

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
 */
void interface(void *pvParameters);

#endif
