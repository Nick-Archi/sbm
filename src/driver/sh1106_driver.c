/*
* @brief Implementation file for the SH1106 OLED
*/

#include <stdlib.h>

#include "spi_transport.h"
#include "SH1106_Commands.h"
#include "sh1106_driver.h"

static void init(Display* display)
{
printf("Dbg: Within init for sh1106 driver .c\n");
    // configure the SH1106
    SpiTransportConfig* ctx = (SpiTransportConfig*)display->ctx; 
    gpio_put(ctx->rst, 0);
    sleep_ms(10);
    gpio_put(ctx->rst, 1);

    sleep_ms(3000);
    
    // send init data to SH1106
    for(int i = 0; i < count_of(init_config_steps); ++i)
    {
        display->transport->send_command(display, init_config_steps[i]);
    }
//    display->transport->send_command(display, init_config_steps, count_of(init_config_steps));

    // turn on display
    display->transport->send_command(display, SH1106_DISPLAYON);
    sleep_ms(500);

}

static void update(Display* display)
{

}
 
static void clear(Display* display)
{
    
}

static void write_string(Display* display)
{

}

const DisplayDriver SH1106_DRIVER = {
    .init = init,
    .update = update,
    .clear = clear,
    .write_string = write_string 
};

void setup_display_sh1106(Display* display, size_t num_of_pgs, size_t buffer_size, uint8_t* buffer)
{
    // buffer setup
    display->buffer.num_of_pgs = num_of_pgs;
    display->buffer.pgs = (page_desc*)calloc(display->buffer.num_of_pgs, sizeof(paged_buffer));

    for(size_t i = 0; i < num_of_pgs; ++i)
    {
        display->buffer.pgs[i].pg = &buffer[i * display->width];
    }

    // setup of Display objects
    display->driver = &SH1106_DRIVER;
    display->transport = &SPI_TRANSPORT;
}
