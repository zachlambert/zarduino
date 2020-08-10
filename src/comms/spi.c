#include "comms/spi.h"
#include "core/regs.h"
#include "core/pin_data.h"
#include "comms/uart.h"

#include <avr/io.h>
#include <avr/interrupt.h>

SPIConfig spi_create_config(void)
{
    SPIConfig config = {};
    config.frequency = SPI_FREQUENCY_DIV_128;
    return config;
}

void spi_init_master(SPIConfig *config)
{
    // !! The dedicated SS pin doesn't have to be used
    // however, it cannot get into an INPUT LOW state,
    // otherwise the device automatically becomes a slave.
    // Therefore, set as output before enabling SPI
    gpio_mode_output(PIN_SS);

    reg_write_bit(&SPCR, SPE, 1);
    reg_write_bit(&SPCR, DORD, config->data_order);
    reg_write_bit(&SPCR, MSTR, 1);
    reg_write_bit(&SPCR, CPOL, config->polarity);
    reg_write_bit(&SPCR, CPHA, config->phase);
    reg_write_bit(&SPCR, SPR0, (config->frequency & 0b001));
    reg_write_bit(&SPCR, SPR1, (config->frequency & 0b010)>>1);
    reg_write_bit(&SPSR, SPI2X, (config->frequency & 0b100)>>2);

    // User has to choose their own chip select pin
    // and set it as an output
    gpio_mode_output(PIN_SCK);
    gpio_mode_output(PIN_MOSI);
}

uint8_t spi_transfer(uint8_t data_in)
{
    SPDR = data_in;
    asm volatile("nop");
    while(!reg_read_bit(&SPSR, SPIF));
    return SPDR;
}
