#include "module/radio.h"
#include "comms/spi.h"
#include "core/pins.h"
#include "comms/uart.h"

#include "module/radio_constants.h"

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

    // Set retries to delay 5, count 15
    // write_register(0x04, (5 & 0xf) << 4 | (15 & 0xf));

    // Set data rate to 1MBPS
    // uint8_t setup = radio_read_register(config, 0x06);
    // setup &= ~(1<<5 | 1<<3);
    // txDelay = 85;
    // write_register(0x06, setup);

}

void radio_set_mode_rx(RadioConfig *config)
{
    uint8_t config_value = radio_register_read(
            config, register_CONFIG
    );
    config_value |= (1 << 0) | (1 << 1);
    radio_register_write(config, 0x00, config_value);
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

uint8_t radio_register_read(RadioConfig *config, uint8_t address)
{
    uint8_t data_in[] = {
        (address & 0x1F),
        0xFF
    };
    uint8_t data_out[2];
    spi_transfer_bytes(config->CSN, data_in, data_out, 2);
    return data_out[1];
}

void radio_register_write(RadioConfig *config, uint8_t address, uint8_t value)
{
    uint8_t data_in[] = {
        (address & 0x1F) | 1<<5,
        value
    };
    spi_transfer_bytes(config->CSN, data_in, 0, 2);
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
