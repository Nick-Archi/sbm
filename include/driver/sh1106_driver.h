/*
* @brief Header regarding interaction with display corresponding to 
* SH1106 OLED
*/

#ifndef _SH1106_DRIVER_H_
#define _SH1106_DRIVER_H_

#include "driver.h"

#define OLED_PICO    PICO_DEFAULT_SPI_TX_PIN
#define OLED_CS      PICO_DEFAULT_SPI_CSN_PIN
#define OLED_CLK     PICO_DEFAULT_SPI_SCK_PIN
#define OLED_POCI    PICO_DEFAULT_SPI_RX_PIN // DC pin on SH1106
#define OLED_RST     20 // GPIO20 on RP2350

#define SPI_PORT    spi0
#define BAUD        (1000 * 1000)

/*
* @brief Object to implementation for sh1106 display
*/
extern const DisplayDriver SH1106_DRIVER;

/*
* @brief function to initialize the sh1106 display
*
* @details
* The display object should already have 
* @param display, object pretaining to sh1106 obj
* @param num_of_pgs, number of pages
* @param buffer_size, size of underlying data buffer  
*/
void setup_display_sh1106(Display* display, size_t num_of_pgs, size_t buffer_size, uint8_t* buffer);

#endif // _SH1106_DRIVER_H_
