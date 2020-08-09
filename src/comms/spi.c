#include "comms/spi.h"
#include "core/regs.h"

#include <avr/io.h>

void (*callback)(uint8_t);
ISR(SPI_STC_vect)
{
    callback(SPDR);
}

SPIConfig spi_create_config(void)
{
    SPIConfig config = {};
    // Leave deafult 0 values
    return config;
}

void spi_init(SPIConfig *config)
{
    reg_write_bit(&SPCR, SPE, 1);
    reg_write_bit(&SPCR, DORD, config->data_order);
    reg_write_bit(&SPCR, MSTR, 1);
    reg_write_bit(&SPCR, CPOL, config->polarity);
    reg_write_bit(&SPCR, CPHA, config->phase);
    if (config->callback) {
        callback = config->callback;
        reg_write_bit(&SPCR, SPIE, 1);
    }
}

uint8_t spi_read_byte(void)
{
    while(!reg_read_bit(&SPSR, SPIF));
    return SPDR;
}

void spi_write_byte(uint8_t data)
{
    SPDR = data;
    while(!reg_read_bit(&SPSR, SPIF));
}

void spi_read_bytes(uint8_t *data, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        while(!reg_read_bit(&SPSR, SPIF));
        data[i] = SPDR;
    }
}

void spi_write_bytes(uint8_t *data, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        SPDR = data[i];
        while(!reg_read_bit(&SPSR, SPIF));
    }
}
