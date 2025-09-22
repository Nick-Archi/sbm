/*
* @brief Interface to interact with the underlying frame buffer
* for appropriate display object
*/

#ifndef _DRIVER_H_
#define _DRIVER_H_

#include "display.h"

/*
* @brief struct containing functions pertaining to display 
* @details
* This struct will points to the underlying functions that 
* allows proper communication with the display 
*/
typedef struct DisplayDriver
{
    void (*init)(Display* display); /**< initialization implementation*/
    void (*update)(Display* display); /**< update implementation */
    void (*clear)(Display* display); /**< clear implementation */
    void (*write_string)(Display* display); /**< write_string implementation */
}DisplayDriver;

#endif // _DRIVER_H_
