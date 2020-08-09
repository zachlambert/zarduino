#include "modules/radio.h"
#include "comms/spi.h"
#include "core/pins.h"

RadioConfig radio_create_config(void);

void radio_init(RadioConfig *config)
{
    gpio_mode_output(config->CE);
    gpio_mode_output(config->CSN);
    gpio_mode_input(config->IRQ);
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

void radio_send_command(uint8_t command)
{
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
