/*
* @brief Declaration for display module 
* @details
* This module deals with creating a display object for managing 
* specific displays. It creates functionality to initialize, update, 
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
* 
*/
typedef struct
{
    uint8_t* pg; /**< pointer to underlying data buffer */
    uint8_t dirty; /**< dirtied (1) */
    uint8_t dirty_start_col; /**< start offset of write */
    uint8_t dirty_end_col; /**< end offset of write */
}page_desc;

/*
* @brief This struct essentially splits the data buffer into pages 
*/
typedef struct 
{
    page_desc* pgs; /**< pointer to allocated buffer */
    int num_of_pgs; /**< number of pages in buffer */
}paged_buffer;

/*
* @brief struct that keeps track of info & function for underlying display
*/
typedef struct Display
{
    const DisplayDriver* driver; /**< buffer interaction */
    const DisplayTransport* transport; /**< underlying transport calls */
    paged_buffer buffer; /**< buffer */
    uint8_t width; /**< width of display */
    uint8_t height; /**< height of display */
    void* ctx; /**< context info regarding transport calls */
}Display;

void display_init(Display* display);
void display_update(Display* display);
void display_clear(Display* display);
void display_write_string(Display* display, char* str, size_t pg_start, size_t pos_start, size_t total_size);

#endif // _DISPLAY_H_
