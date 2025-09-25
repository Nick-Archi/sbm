/*
* @brief Implementation file for the SH1106 OLED
*/

#include <stdlib.h>

#include "spi_transport.h"
#include "SH1106_Commands.h"
#include "sh1106_driver.h"

/*
* @brief function to set the lower & upper column addresses
*
* Read section on the Column Address pg 15, 19 in 
* Sino Wealth SH1106 PDF
*
* @param display, pointer to display object
* @param col, column in page (0 <= col <= 127)
*/
static void set_column_address(Display* display, uint8_t col)
{
    /* 
    * this is special for the SH1106, seems it needs to be offset
    * otherwise text on the OLED will be improper.
    */
    col += 2;
    uint8_t cmd = (0x10 | (col >> 4));
    // set the higher column address?
    display->transport->send_command(display, &cmd, 1);     
    // set the lower column address?
    cmd = (0x00 | (col & 0x0F)); 
    display->transport->send_command(display, &cmd, 1);      
}

static void init(Display* display)
{
    // configure the SH1106
    SpiTransportConfig* ctx = (SpiTransportConfig*)display->ctx; 
    gpio_put(ctx->rst, 0);
    sleep_ms(10);
    gpio_put(ctx->rst, 1);

    sleep_ms(3000);
    
    // send init data to SH1106
    display->transport->send_command(display, init_config_steps, count_of(init_config_steps));

    // turn on display
    uint8_t cmd = SH1106_DISPLAYON;
    display->transport->send_command(display, &cmd, 1);
    sleep_ms(500);

}

static void update(Display* display)
{
    for(size_t idx = 0; idx < display->pg_buf->num_of_pages; ++idx)
    {
        if(!display->pg_buf->page[idx].dirty)
        {
            continue;
        }

        uint8_t cmd = SH1106_PAGE_OFFSET(idx);
        display->transport->send_command(display, &cmd, 1);
        set_column_address(display, 0);
       
//[TODO] likely update this so that it can write only the number of bytes that have been changed? 
        for(uint8_t byte = 0; byte < SH1106_WIDTH; ++byte)
        {
            /*
            * This is performing a type of offset. Think of it as
            * addressing a 2D array as a 1D vector.
            * ex) page = 1, i = 0, [1(page) * 128 + 0], access buffer[128] through
            * buffer[256], that's 128 bytes for page 1...
            */

            display->transport->send_data(display, &display->pg_buf->page[idx].buf[byte], 1); 
        }

        display->pg_buf->page[idx].dirty = 0;
        display->pg_buf->page[idx].dirty_start_col = UINT8_MAX;
        display->pg_buf->page[idx].dirty_end_col = 0;
    }
}
 
static void clear(Display* display)
{
    memset(display->pg_buf->page[0].buf, 0x00, SH1106_BYTES);
    for(size_t i = 0; i < SH1106_PAGES; ++i)
    {
        display->pg_buf->page[i].dirty = 1;
    } 
}

const DisplayDriver SH1106_DRIVER = {
    .init = init,
    .update = update,
    .clear = clear,
};

void setup_display_sh1106(Display* display, DisplayPages* desc, DisplayPageInfo* info, size_t width, size_t height, size_t num_of_pages, uint8_t* buffer)
{
    // setup of Display objects
    display->driver = &SH1106_DRIVER;
    display->transport = &SPI_TRANSPORT;

    display->width = width;
    display->height = height;
    
    // buffer setup
    display->pg_buf = desc;
    display->pg_buf->num_of_pages = num_of_pages;
    display->pg_buf->page = info;

    // initialize each page 
    for(size_t i = 0; i < num_of_pages; ++i)
    {
        display->pg_buf->page[i].buf = &buffer[SH1106_WIDTH * i];
        display->pg_buf->page[i].dirty = 0U;
        display->pg_buf->page[i].dirty_start_col = UINT8_MAX;
        display->pg_buf->page[i].dirty_end_col = 0U;
    }
}

