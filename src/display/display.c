/*
* @brief Implementation for display module 
*/

#include "display.h"
#include "driver.h"
#include "char_bitmap.h"
#include "lame_bitmap.h"
#include "digi_bitmap_2.h"

/*
* @brief Update the dirty info for page 
*
* @param page, page to update
* @param offset, location of where the writing is occurring 
*
* @return void
*/
static void update_dirty_page(DisplayPageInfo* page, size_t offset)
{
    page->dirty = 1;
    page->dirty_start_col = (page->dirty_start_col > offset) ? offset : page->dirty_start_col;
    page->dirty_end_col = (page->dirty_end_col < offset) ? offset : page->dirty_end_col;
}

/*
* @brief Write to specific page in buffer 
*
* @param display, pointer to display object
* @param data, Address of data to copy into buffer.
* @param pg, page number for indexing into buffer.
* @param offset, position to write to in selected page.
* @param size, number of bytes to copy, must be <= (page size).
*
* @return void
*/
static void write_to_page(Display* display, const uint8_t* data, size_t pg, size_t offset, size_t size)
{
    /* 
    * [TODO] might need to update this size check...since it's 
    * possible to write to all pages with a single write..
    */

    /*
    * Ensure you're not writing past the boundary of the page buffer.
    * size > ((display.width * display.height)/8), this represents the total
    * size of buffer in bytes.
    * offset > (display.width/8), represents how many characters can fit on
    * screen. 
    */
    if(data == NULL || pg < 1 || pg > display->pg_buf->num_of_pages || size > ((display->width*display->height)/8) || offset > (display->width/8))
    { return; }

    update_dirty_page(&display->pg_buf->page[pg-1], offset);
    memcpy(display->pg_buf->page[pg - 1].buf + (offset * 8), data, size);
}

/*
* @brief Convert a character into the bitmap value 
*
* @param val, ascii value for a character
*
* @return uint8_t*, address to char map 
*/
static const uint8_t* char_to_bitmap(unsigned char val)
{
    const uint8_t* addr = NULL;

    if(val == ' ')
    {
        val -= 32;
        addr = &spec_bitmap[val][0];
    }

    if(val >= '0' && val <= '9')
    {
        val -= 48;
        addr = &digi_bitmap_2[val][0];
    }
    
    if(val >= 'A' && val <= 'z') 
    {
        if(val >= 'a' && val <= 'z')
        {
            val -= 32;
        }

        val -= 65;
        addr = &char_bitmap[val][0];
    }

    return addr;
}

void display_init(Display* display)
{
    display->driver->init(display);
}

void display_update(Display* display)
{
    display->driver->update(display);
}

void display_clear(Display* display)
{
    display->driver->clear(display);
}

void display_write_string(Display* display, const unsigned char* str, size_t pg_start, size_t pos_start, size_t total_size)
{
    if(str == NULL || pg_start > display->pg_buf->num_of_pages || 
        pg_start < 1 || pos_start > (display->width / 8) || 
        total_size > display->width)
    { return; }

    size_t offset = pos_start;
    for(size_t idx = 0; idx < (total_size/8); ++idx)
    {
        const uint8_t* addr = char_to_bitmap(str[idx]);
        if(addr == NULL)
        { return; }
        
        write_to_page(display, addr, pg_start, offset, 8);
        offset++;
    }
}
