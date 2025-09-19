/*
* @brief Interface for defining functions & structs for interacting with
* underlying display.
*/

#ifndef _TRANSPORT_H_
#define _TRANSPORT_H_

#include "display.h"

typedef struct DisplayTransport
{
    void (*send_command)(Display* display, uint8_t cmd);
    void (*send_data)(Display* display, const uint8_t* data, size_t len);
}DisplayTransport;

#endif // _TRANSPORT_H_
