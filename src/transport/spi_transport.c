/*
* @brief Implementation for spi communication via pico-sdk
*/

#include "spi_transport.h"

static void spi_send_command(Display* display, uint8_t cmd)
{

}

static void spi_send_data(Display* display, const uint8_t* data, size_t len)
{

}

const DisplayTransport SPI_TRANSPORT = {
    .send_command = spi_send_command,
    .send_data = spi_send_data,
};
