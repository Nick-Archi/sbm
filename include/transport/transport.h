/* * @brief Interface for defining functions & structs for interacting with
* underlying display.
*/

#ifndef _TRANSPORT_H_
#define _TRANSPORT_H_

#include "display.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
* @brief struct for implementation of transport functionality
*/
typedef struct DisplayTransport
{
    void (*send_command)(Display* display, const uint8_t* data, size_t len); /**< send command to display */
    void (*send_data)(Display* display, const uint8_t* data, size_t len); /** send data to display */
}DisplayTransport;

#ifdef __cplusplus
}
#endif

#endif // _TRANSPORT_H_
