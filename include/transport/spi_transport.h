/*
* @brief Definition for interacting with display via SPI calls
*/

#ifndef _SPI_TRANSPORT_H_
#define _SPI_TRANSPORT_H_

// pico-sdk calls
#include "hardware/spi.h"

#include "transport.h"

typedef struct SpiTransportConfig
{
    spi_inst_t* spi;
    uint8_t cs;
    uint8_t clk;
    uint8_t poci;
    uint8_t pico;
    uint8_t rst;
    uint32_t baud;
}SpiTransportConfig;

void spi_transport_init(const SpiTransportConfig* cfg);

#endif // _SPI_TRANSPORT_H_
