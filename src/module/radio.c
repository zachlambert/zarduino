#include "module/radio.h"
#include "comms/spi.h"
#include "core/pins.h"
#include "comms/uart.h"

RadioConfig radio_create_config(void)
{
    RadioConfig config = {};
    // Put some default values
    return config;
}

void radio_init(RadioConfig *config)
{
    gpio_mode_output(config->CE);
    gpio_mode_output(config->CSN);
    gpio_mode_input(config->IRQ);
    gpio_write(config->CSN, 1);
}

void radio_set_mode_rx(RadioConfig *config)
{
    // bit PWR_UP = 1
    // bit PRIM_RX = 1
    // pin CE = 1
    gpio_write(config->CSN, 1);
}

void radio_set_mode_tx(RadioConfig *config)
{
    // bit PWR_UP = 1
    // bit PRIM_RX = 0
    // pin CE = 1
    // Put a payload in the TX FIFO
    // High pulse on CE for over 10us
}

uint8_t radio_register_read(
    RadioConfig *config,
    uint8_t register_address)
{
    // Send read register command first, then NOP (0xFF)
    // while reading data
    // Currently, only reads one byte
    uint8_t command = register_address & 0b00011111;
    uint8_t data_in[] = {command, 0xFF};
    uint8_t data_out[2];
    // spi_transfer_bytes(config->CSN, data_in, data_out, 2);
    // status = data_out[0]
    // read value = data_out[1]
    // If have data_in[2] = 0xFF, etc
    // then get more values in data_out[2], etc
    return data_out[1];
}

void radio_register_write(
    RadioConfig *config,
    uint8_t register_address,
    uint8_t value
){
    uint8_t command = (register_address & 0b00011111) | 1<<5;
    uint8_t data_in[] = {command, value};
    printf("SPI write\n");
    // spi_write_bytes(config->CSN, data_in, 2);
}

// RF channel frequency is given by:
// F0 = 2400 + RF_CH[MHz}

// IRQ: Active low, conrolled by three maskable interrupt sources
// CE: Active high, activates chip (in RX or TX mode)
// CSN: SPI chip select
// SCK, MOSI, MISO: SPI

// SPI Commands:
// Start: High->Low transition on CSN like usual
// When sending data over MOSI, the STATUS register is shifted
// out on the MISO pin
// Serial shifting SPI commands have this format:
// - <Command word>: MSBit to LSBit (one byte)
// - <Data bytes>: LSByte to MSByte, MSBit in each byte first
