/*
* @brief Definition for interacting with display via SPI calls
*/

#ifndef _SPI_TRANSPORT_H_
#define _SPI_TRANSPORT_H_

// pico-sdk includes 
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "pico/time.h"

#include "transport.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
* instance for SPI implementation
*/
extern const DisplayTransport SPI_TRANSPORT; 

/*
* @brief spi setup for display object
*/
typedef struct SpiTransportConfig
{
    spi_inst_t* spi; /**< pointer to SPI instance */
    uint8_t cs; /**< chip select */
    uint8_t clk; /**< clock */
    uint8_t dc; /**< cmd select */
    uint8_t pico; /**< dc */ 
    uint8_t rst; /**< reset */
    uint32_t baud; /**< baud rate */
}SpiTransportConfig;

/*
* @brief initialize & configure underlying SPI interface
*/
void spi_transport_init(const SpiTransportConfig* cfg);

#ifdef __cplusplus
}
#endif

#endif // _SPI_TRANSPORT_H_
