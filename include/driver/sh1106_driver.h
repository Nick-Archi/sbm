/*
* @brief Header regarding interaction with display corresponding to 
* SH1106 128x64 OLED
*/

#ifndef _SH1106_DRIVER_H_
#define _SH1106_DRIVER_H_

#include "driver.h"

#define OLED_PICO    PICO_DEFAULT_SPI_TX_PIN
#define OLED_CS      PICO_DEFAULT_SPI_CSN_PIN
#define OLED_CLK     PICO_DEFAULT_SPI_SCK_PIN
#define OLED_POCI    PICO_DEFAULT_SPI_RX_PIN // DC pin on SH1106
#define OLED_RST     20U // GPIO20 on RP2350

#define SPI_PORT    spi0
#define BAUD        (1000 * 1000)

#define SH1106_PAGES 8U
#define SH1106_WIDTH 128U
#define SH1106_HEIGHT 64U
#define SH1106_BYTES ((SH1106_WIDTH * SH1106_HEIGHT) / 8)

/*
* @brief Instance to implementation for sh1106 display
*/
extern const DisplayDriver SH1106_DRIVER;

/*
* @brief function to initialize the sh1106 display
*
* @details
* The display object should already have 
* @param display, object pretaining to sh1106 obj
* @param desc, allocated DisplayPage object 
* @param info, allocated DisplayPageInfo object 
* @param num_of_pages, number of pages for display 
* @param buffer, allocated memory region for oled screen 
*/
void setup_display_sh1106(Display* display, DisplayPages* desc, DisplayPageInfo* info, size_t num_of_pages, uint8_t* buffer);

#endif // _SH1106_DRIVER_H_
