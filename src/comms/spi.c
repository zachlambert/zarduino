#include "comms/spi.h"
#include "core/regs.h"
#include "core/pin_data.h"

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

void spi_init_master(SPIConfig *config)
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
    // User has to choose their own chip select pin
    // and set it as an output
    gpio_mode_output(PIN_SCK);
    gpio_mode_output(PIN_MOSI);
}

uint8_t spi_read_byte(Pin CS)
{
    uint8_t value;
    gpio_write(CS, 0);
    while(!reg_read_bit(&SPSR, SPIF));
    value = SPDR;
    gpio_write(CS, 1);
    return value;
}

void spi_write_byte(Pin CS, uint8_t data)
{
    gpio_write(CS, 0);
    SPDR = data;
    while(!reg_read_bit(&SPSR, SPIF));
    gpio_write(CS, 1);
}

void spi_read_bytes(Pin CS, uint8_t *data, size_t len)
{
    gpio_write(CS, 0);
    for (size_t i = 0; i < len; i++) {
        while(!reg_read_bit(&SPSR, SPIF));
        data[i] = SPDR;
    }
    gpio_write(CS, 1);
}

void spi_write_bytes(Pin CS, uint8_t *data, size_t len)
{
    gpio_write(CS, 0);
    for (size_t i = 0; i < len; i++) {
        SPDR = data[i];
        while(!reg_read_bit(&SPSR, SPIF));
    }
    gpio_write(CS, 1);
}

uint8_t spi_transfer_byte(Pin CS, uint8_t data_in)
{
    gpio_write(CS, 0);
    SPDR = data_in;
    while(!reg_read_bit(&SPSR, SPIF));
    uint8_t data_out = SPDR;
    gpio_write(CS, 1);
    return data_out;
}

void spi_transfer_bytes(
    Pin CS, uint8_t *data_in, uint8_t *data_out, size_t len
){
    gpio_write(CS, 0);
    for (size_t i = 0; i < len; i++) {
        SPDR = data_in[i];
        while(!reg_read_bit(&SPSR, SPIF));
        data_out[i] = SPDR;
    }
    gpio_write(CS, 1);
}
