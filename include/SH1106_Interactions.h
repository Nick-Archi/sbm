/*
* Description:
* Header file for SH1106 initialization and usage.
* 
*/

#ifndef _SH1106_INTERACTIONS_H_   
#define _SH1106_INTERACTIONS_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"

/*
* SH1106 Display settings
* I am only using a 128x64 display for now
*/
#define SH1106_128_64

#if defined SH1106_128_64
  #define PAGES                  (8)
  #define WIDTH                  (128)
  #define HEIGHT                 (64)
  #define BYTES                  ((HEIGHT * WIDTH) / 8)
#endif


// SPI Configuration for Raspberry Pi RP2350
#define PICO    PICO_DEFAULT_SPI_TX_PIN
#define CS      PICO_DEFAULT_SPI_CSN_PIN
#define CLK     PICO_DEFAULT_SPI_SCK_PIN
#define POCI    PICO_DEFAULT_SPI_RX_PIN // DC pin on SH1106
#define RST     20 // GPIO20 on RP2350

#define SPI_PORT    spi0
#define BAUD        (1000 * 1000)

/*
* @enum 
* @brief Enum to represent the initialization state of the SH1106 object.
*
*/
typedef enum 
{
    UNINIT = 0,     /**< object has passed through parameter initialzation */
    INIT = 1,       /**< object has setup SPI comm */
    CONFIGED = 2,   /**<  */
}SH1106_state;

/*
* @struct page_desc_t
* @brief Track individual page info
* 
* Typedef'd struct assisting in tracking individual info:
* buffer address, page dirtied info,  
*/
typedef struct page_desc_t
{
    uint8_t* page;
    uint8_t dirtied; /** Dirtied (1), Clean (0) */
    uint8_t dirty_start_col; /** Start offset of dirty */
    uint8_t dirty_end_col; /** End offset of dirty */
}page_desc;

/*
* Typedef'd struct assist accessing pages(0-7) in buffer 
*/
typedef struct paged_buffer_t
{
    page_desc pages[8];
}paged_buffer;

/*
* Object that will hold configuration info for an SH1106 OLED 
*/
typedef struct SH1106_t
{
    uint8_t dc; /**< Data/Command: set 0: commands, set 1: data */
    uint8_t rst;
    uint8_t cs;
    uint8_t pico;
    uint8_t clk;
    uint8_t* buffer; 
    SH1106_state init; // used to determine if oled struct initialized
}SH1106;

#ifdef __cplusplus
extern "C" {
#endif

/*
* @brief Initializer function to create and initalize
* an SH1106 object
*
* @param dc, GPIO pin for Direct Comand
* @param rst, GPIO pin for SPI Reset
* @param cs, GPIO pin for 
* @param pico, GPIO pin for Controller Out Peripheral In
* @param clk, GPIO pin for clock
*
* @return SH1106*, pointer to object
*/
void init_SH1106(
    uint8_t dc, 
    uint8_t rst, 
    uint8_t cs,
    uint8_t pico,
    uint8_t clk
);

/*
* @brief Function used to initalize & configure SPI
*
* Configures SPI and utilizes the SH1106 object
* to initialize and configure the GPIO pins on the Pico2
*
* @return void
*/
void initialize_spi();

/*
* @brief Initialize static paged buffer variable 
*
* Initialize static paged buffer variable, within 
* corresponding .c file, to point to address of 
* indicies that are multiples of 128. Essentially
* each page is 128 bits so this is the address of
* each page. 
*
* @return void
*/
void init_page_buffer();

/*
* @brief Runs the power-up sequence for the SH1106.
*
* Following the power-up sequence from the Adafruit SH1106 documentation
* and code found on their github. This function should run after
* setting up SPI communication.
*
* @return void
*/
void begin_sh1106();

/*
* @brief Executes reset steps for the SH1106.
*
* @return void
*/
void reset_sh1106();

/*
* @brief Executes the powerup & setup sequence for the SH1106.
*
* These steps are taken from the Adafruit SH1106 doc & code
* examples.
*
* Pre-reqs: initialized SPI communication
*
* @return void
*/
void configure_sh1106();

/*
* @brief Sends a command to the SH1106 via SPI.
*
* Pre-reqs: initialized SPI communication
*
* @param cmd, byte to be sent to OLED as command.
*
* @return void
*/
void send_command_sh1106(uint8_t cmd);

/*
* @brief Sends a byte of data to the SH1106 via SPI.
*
* Pre-reqs: initialized SPI communication
*
* @param data, byte to be sent to OLED as data.
*
* @return void
*/
void send_data_sh1106(uint8_t* data);

/*
* @brief Send buffer data to OLED to update the display 
*
* Pre-reqs: initialized SPI communication
*
* @return void
*/
void update_sh1106();

/*
* @brief Set the lower & upper column addresses 
*
* For more information, read the section on The Column Address pg 15, 19
* in the Sino Wealth SH1106 pdf
*
* @param col, Column between (0 - 127).
*
* @return void
*/
void set_column_address(uint8_t col);

/*
* @brief Write to specific page in buffer 
*
* @param data, Address of data to copy into buffer.
* @param pg, page number (1-8) for indexing into buffer.
* @param offset, position to write to in selected page.
* @param size, number of bytes to copy, must be <= 1024 (page size).
*
* @return void
* @notes each char is 8x8 = 64. Only 16 chars can fit on the screen. So 
* 16 * 64 = 1024. This is the max per page
*/
void write_to_page(const uint8_t* data, size_t pg, size_t offset, size_t size);

/*
* @brief Write to specific page in buffer 
*
* @param val, data to write
* @param pg_start, page to start write in
* @param pos_start, 
* @param total_size, number of bytes to copy, must be < 128 (page size).
*
* @return void
*/
void write_string(const unsigned char* val, size_t pg_start, size_t pos_start, size_t total_size);

/*
* @brief Update the dirty info for page 
*
* @param pg, page where update occurs 
* @param offset, location of where the writing is occurring 
*
* @return void
*/
void update_dirty_page(size_t pg, size_t offset);

/*
* @brief Clear the buffer to the OLED and write it to the screen.
*
* @return void
*/
void clear_buffer();

/*
* @brief Set the buffer to the OLED and write it to the screen.
*
* @return void
*/
void set_buffer();


/*
* @brief HELPER Function to write a character to a page.
*
* @param val, character
* @param page, page number to write to
*
* @return void
*/
void insert_char(unsigned char val, size_t page);

#ifdef __cplusplus
}
#endif

#endif // _SH1106_INTERACTIONS_H_
