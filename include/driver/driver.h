/*
* @brief Interface to interact with the underlying frame buffer
* for appropriate display object
*/

#ifndef _DRIVER_H_
#define _DRIVER_H_

#include "display.h"

typedef struct DisplayDriver
{
    void (*init)(Display* display);
    void (*update)(Display* display);
    void (*clear)(Display* display);
    void (*write_string)(Display* display);
}DisplayDriver;

#endif // _DRIVER_H_
