/*
* @brief Declaration for display module 
* @details
* This module deals with creating a display object for managing 
* specific displays. It mandates functionality to initialize, update, 
* and write strings to the managed display. 
*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <stddef.h>
#include <stdint.h>

// forward declare driver and transport structs
typedef struct DisplayDriver DisplayDriver;
typedef struct DisplayTransport DisplayTransport;

/*
* @brief dirty page info
* @details
* This struct deals with keeping track of info
* regarding if a page has been written (dirtied)
*/
typedef struct
{
    uint8_t* buf; /**< pointer to underlying data buffer */
    uint8_t dirty; /**< dirtied (1) */
    uint8_t dirty_start_col; /**< start offset of write */
    uint8_t dirty_end_col; /**< end offset of write */
}DisplayPageInfo;

/*
* @brief This struct essentially splits the data buffer into pages 
*/
typedef struct 
{
    DisplayPageInfo* page; /**< pointer to page descriptor */
    int num_of_pages; /**< number of descriptor */
}DisplayPages;

/*
* @brief struct that keeps track of info & function for underlying display
*/
typedef struct Display
{
    const DisplayDriver* driver; /**< pointer to display obj */
    const DisplayTransport* transport; /**< pointer to transport obj */
    DisplayPages* pg_buf; /**< pointer to buffer obj */
    void* ctx; /**< context info regarding transport calls */
}Display;

/*
* @brief initialize the display
*
* @details This function will call the necessary steps/sequence 
* to initialize the display.
*
* @param display, pointer to display object 
*/
void display_init(Display* display);

/*
* @brief Update the display
*
* @details This function will call the necessary steps/sequence 
* to update the display.
*
* @param display, pointer to display object 
*/
void display_update(Display* display);

/*
* @brief Clear the display buffer
*
* @param display, pointer to display object 
*/
void display_clear(Display* display);

/*
* @brief Writes a string to the display buffer
*
* @param str, data to write 
* @param pg_start, starting page to write into
* @param pos_start, starting position in page
* @param total_size, total number of bytes to write
*/
void display_write_string(Display* display, char* str, size_t pg_start, size_t pos_start, size_t total_size);

#endif // _DISPLAY_H_
