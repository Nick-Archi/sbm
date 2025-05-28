/*
* Objective: This file ultimately configures the SH1106 OLED.
*
* I analyzed the steps found in the Adafruit_SH110x project: https://github.com/adafruit/Adafruit_SH110x/tree/master
* 
*/

#include "SH1106_Interactions.h"
#include "SH1106_Commands.h"
#include "lame_bitmap.h"
#include "char_bitmap.h"
#include "digi_bitmap_2.h"

extern SH1106 oled;

/*
* Array of bytes used to hold data to write to OLED
*/
static uint8_t buffer[BYTES]; // divide by 8 b/c accounting for byte per pixel

/*
* Assist with writing to specific pages in OLED buffer
*/
static paged_buffer pg_buf;

void init_SH1106(
    uint8_t dc, 
    uint8_t rst, 
    uint8_t cs,
    uint8_t pico,
    uint8_t clk
)
{
    oled.dc = dc;
    oled.rst = rst;
    oled.cs = cs;
    oled.pico = pico;
    oled.clk = clk;
    oled.cs = cs;
    oled.buffer = buffer;
    oled.init = UNINIT;

    printf("DEBUG0\n");
    printf("oled.dc = %d\noled.rst = %d\noled.cs = %d\noled.pico = %d\n\
oled.clk = %d\noled.init = %d\n",\
oled.dc, oled.rst, oled.cs, oled.pico, oled.clk, oled.init);
}

void initialize_spi()
{
    if(oled.init != UNINIT)
    {
        printf("Issue During Initialization\n");
        while(1);
    } 
 
    // Reset the SH1106.
    gpio_init(oled.rst);
    gpio_set_dir(oled.rst, GPIO_OUT);
    sleep_ms(10); // delay for Vdd to stabilize 

    // initialize the CS pin & set pin dir
    gpio_init(oled.cs);
    gpio_set_dir(oled.cs, GPIO_OUT);
    gpio_put(oled.cs, 1); // CS -> high, no communication yet
    
    spi_init(SPI_PORT, BAUD);
    printf("DEBUG: BAUD rate configured to = %d\n", spi_get_baudrate(SPI_PORT)); 
    
    // initialize & config gpio pins 
    gpio_set_function(oled.pico, GPIO_FUNC_SPI);
    gpio_set_function(oled.clk, GPIO_FUNC_SPI);

    // initialize the DC pin & set pin dir
    gpio_init(oled.dc);
    gpio_set_dir(oled.dc, GPIO_OUT);
    gpio_put(oled.dc, 0);

    init_page_buffer();

    oled.init = INIT;
}

void init_page_buffer()
{
    for(int i = 0; i < 8; ++i)
    {
        pg_buf.pages[i].page = &buffer[(i * 128U)];
        pg_buf.pages[i].dirtied = 0U;
        pg_buf.pages[i].dirty_start_col= UINT8_MAX;
        pg_buf.pages[i].dirty_end_col = 0U;
    }
}

void configure_SH1106()
{
    if(oled.init != INIT)
    {
        printf("Ada struct not initialized\n");
        while(1);
    }

    // bring SH1106 out of Reset
    reset_sh1106();

    sleep_ms(3000);
    // send data over from array
    for(int i = 0; i < sizeof(init_config_steps)/sizeof(uint8_t); i++)
    {
        send_command_sh1106(init_config_steps[i]);
    }

    // send over SET_DISPLAY_ON & delay
    send_command_sh1106(SH1106_DISPLAYON);
    sleep_ms(500);
}

void begin_sh1106()
{
    // initialize the SPI pins on pico2
    initialize_spi();

    // execute the Vdd/Vcc off state -> initial settings configuration steps
    configure_SH1106();   
}

void reset_sh1106()
{
    if(oled.init != INIT)
    {
        printf("Issue w/ Initialization. Not resetting\n");
        while(1);
    }

    gpio_put(oled.rst, 0);
    sleep_ms(10);
    gpio_put(oled.rst, 1);
    sleep_ms(10);
}

void send_command_sh1106(uint8_t cmd)
{
    if(oled.init != INIT)
    {
        printf("Issue with Initialization. Not sending cmd data\n");
        while(1);
    }

    gpio_put(oled.dc, 0); // Cmd mode
    gpio_put(oled.cs, 0);
    sleep_ms(10);
    spi_write_blocking(SPI_PORT, &cmd, 1);
    gpio_put(oled.cs, 1);
}

void send_data_sh1106(uint8_t* data)
{
    if(oled.init != INIT)
    {
        printf("Issue with Initialization. Not sending data\n");
        while(1);
    }

    gpio_put(oled.dc, 1); // Data mode
    gpio_put(oled.cs, 0);
    sleep_ms(10);
    spi_write_blocking(SPI_PORT, data, 1);
    gpio_put(oled.cs, 1);
}

void update_sh1106()
{
    if(oled.init != INIT)
    {
        printf("Ada struct not initialized\n");
        while(1);
    }

    for(uint8_t page = 0; page < 8; page++)
    {
        if(!pg_buf.pages[page].dirtied)
        {
            continue;
        }

        send_command_sh1106(SH1106_PAGE_OFFSET(page));
        set_column_address(0); //[TODO] Possibly refine this so it starts at dirty col start?

        // write each byte in that page to OLED
        for(uint8_t i = 0; i < WIDTH; i++)
        {
            /*
            * This is performing a type of offset. Think of it as
            * addressing a 2D array as a 1D vector.
            * ex) page = 1, i = 0, [1 * 128 + 0], access buffer[128] -
            * buffer[256], that's 128 bytes for page 1...
            */

            send_data_sh1106(&pg_buf.pages[page].page[i]); //[TODO] Possibly refine so that the number of bytes to modify is known exactly between the diryt_start and dirty_end columns?

        }
        pg_buf.pages[page].dirtied = 0;
        pg_buf.pages[page].dirty_start_col = UINT8_MAX;
        pg_buf.pages[page].dirty_end_col = 0;
    }
}

void set_column_address(uint8_t col)
{
    /* 
    * this is special for the SH1106, seems it needs to be offset
    * otherwise text on the OLED will be improper.
    */
    col += 2;
    // set the higher column address?
    send_command_sh1106((0x10 | (col >> 4)));     
    // set the lower column address?
    send_command_sh1106((0x00 | (col & 0x0F)));     
}

void write_to_page(const uint8_t* data, size_t pg, size_t offset, size_t size)
{
    /* 
    * [TODO] might need to update this size check...since it's 
    * possible to write to all pages with a single write..
    */
    if(data == NULL || pg < 1 || pg > 8 || size > 1024 || offset > 16)
    { return; }

    update_dirty_page(pg, offset);
    memcpy(pg_buf.pages[pg - 1].page + (offset * 8), data, size);
}

/*
* @brief Convert a character into the bitmap value 
*
* @param val, ascii value for a character
*
* @return void
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

void write_string(const unsigned char* val, size_t pg_start, size_t pos_start, size_t total_size)
{
    if(val == NULL || pg_start > 8 || pg_start < 1 || pos_start > 16 || total_size > 128)
    { return; }

    size_t offset = pos_start;
    for(size_t idx = 0; idx < (total_size/8); ++idx)
    {
        const uint8_t* addr = char_to_bitmap(val[idx]);
        if(addr == NULL)
        { return; }

        // write a byte to page
        write_to_page(addr, pg_start, offset, 8);
        offset++; 
    }
      
}

void update_dirty_page(size_t pg, size_t offset)
{
    page_desc* desc_ptr = &pg_buf.pages[pg - 1];
    desc_ptr->dirtied = 1;
    desc_ptr->dirty_start_col = (desc_ptr->dirty_start_col > offset) ? offset : desc_ptr->dirty_start_col; 
    desc_ptr->dirty_end_col = (desc_ptr->dirty_end_col < offset) ? offset : desc_ptr->dirty_end_col; 
}

void clear_buffer()
{
    memset(buffer, 0x00, BYTES);
    for(int i = 0; i < 8; ++i)
    {
        send_command_sh1106(SH1106_PAGE_OFFSET(i));
        set_column_address(0); 
        gpio_put(oled.dc, 1); // Cmd mode
        gpio_put(oled.cs, 0);
        sleep_ms(10);
        spi_write_blocking(SPI_PORT, buffer, BYTES/8);
        gpio_put(oled.cs, 1);
    }
}

void set_buffer()
{
    memset(buffer, 0xFF, BYTES);
    for(int i = 0; i < 8; ++i)
    {
        send_command_sh1106(SH1106_PAGE_OFFSET(i));
        set_column_address(0); 
        gpio_put(oled.dc, 1); // Cmd mode
        gpio_put(oled.cs, 0);
        sleep_ms(10);
        spi_write_blocking(SPI_PORT, buffer, BYTES/8);
        gpio_put(oled.cs, 1);
    }
}

void insert_char(unsigned char val, size_t page)
{
    const uint8_t* addr = char_to_bitmap(val);
    if(addr == NULL)
    return;

    write_to_page(addr, page, 0, 8);
}
