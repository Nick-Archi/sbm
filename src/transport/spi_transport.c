/*
* @brief Implementation for spi communication via pico-sdk
*/

#include "spi_transport.h"

static void spi_send_command(Display* display, const uint8_t cmd)
{
printf("Dbg: Writing %d bytes\n", cmd);
    SpiTransportConfig* cfg = (SpiTransportConfig*)display->ctx;
    gpio_put(cfg->dc, 0);
    gpio_put(cfg->cs, 0);
    sleep_ms(10);
    spi_write_blocking(cfg->spi, &cmd, 1);
    gpio_put(cfg->cs, 1);
}

static void spi_send_data(Display* display, const uint8_t data)
{
    SpiTransportConfig* cfg = (SpiTransportConfig*)display->ctx;
    gpio_put(cfg->dc, 1);
    gpio_put(cfg->cs, 0);
    sleep_ms(10);
    spi_write_blocking(cfg->spi, &data, 1);
    gpio_put(cfg->cs, 1);
}

void spi_transport_init(const SpiTransportConfig* cfg)
{
printf("Dbg: SpiTransportConfig: cs = %d, clk = %d, dc = %d, pico = %d, rst = %d, baud = %d\n", cfg->cs, cfg->clk, cfg->dc, cfg->pico, cfg->rst, cfg->baud);
    // reset sh1106
    gpio_init(cfg->rst);
    gpio_set_dir(cfg->rst, GPIO_OUT);
    sleep_ms(10);

    // initialize CS pin & set pin dir
    gpio_init(cfg->cs);
    gpio_set_dir(cfg->cs, GPIO_OUT);
    gpio_put(cfg->cs, 1); // CS -> high, no communication

    spi_init(cfg->spi, cfg->baud);

    // initialize & config gpio pins
    gpio_set_function(cfg->pico, GPIO_FUNC_SPI);
    gpio_set_function(cfg->clk, GPIO_FUNC_SPI);

    // initialize & set DC pin
    gpio_init(cfg->dc);
    gpio_set_dir(cfg->dc, GPIO_OUT);
    gpio_put(cfg->dc, 0);
}

const DisplayTransport SPI_TRANSPORT = {
    .send_command = spi_send_command,
    .send_data = spi_send_data,
};
