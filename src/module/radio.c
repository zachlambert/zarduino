#include "module/radio.h"
#include "comms/spi.h"
#include "core/pins.h"
#include "comms/uart.h"

const uint8_t command_R_REGISTER(uint8_t address) {
    return address & 0x1F;
}
const uint8_t command_W_REGISTER(uint8_t address) {
    return (address & 0x1F) | 1<<5;
}

const uint8_t command_R_RX_PAYLOAD = 0b01100001;
const uint8_t command_W_TX_PAYLOAD = 0b10100000;
const uint8_t command_FLUSH_TX = 0b11100001;
const uint8_t command_FLUSH_RX = 0b11100010;
const uint8_t command_REUSE_TX_PL = 0b11100011;

// Must execute the ACTIVATE command, to enable any
// of the commands below it
const uint8_t command_ACTIVATE = 0b01010000;
const uint8_t command_R_RX_PL_WID = 0b01100000;
const uint8_t command_W_ACK_PAYLOAD(uint8_t pipe) {
    return (pipe & 0x7) | 0b10101000;
}
const uint8_t command_W_TX_PAYLOAD_NO_ACK = 0b10110000;
const uint8_t command_NOP = 0xFF;


const uint8_t register_CONFIG = 0x00;
const uint8_t register_EN_AA = 0x01;
const uint8_t register_EN_RXADDR = 0x02;
const uint8_t register_SETUP_AW = 0x03;
const uint8_t register_SETUP_RETR = 0x04;
const uint8_t register_RF_CH = 0x05;
const uint8_t register_RF_SETUP = 0x06;
const uint8_t register_STATUS = 0x07;
const uint8_t register_OBSERVE_TX = 0x08;
const uint8_t register_CD = 0x09;

const uint8_t register_RX_ADDR_P0 = 0x0A;
const uint8_t register_RX_ADDR_P1 = 0x0B;
const uint8_t register_RX_ADDR_P2 = 0x0C;
const uint8_t register_RX_ADDR_P3 = 0x0D;
const uint8_t register_RX_ADDR_P4 = 0x0E;
const uint8_t register_RX_ADDR_P5 = 0x0F;
const uint8_t register_TX_ADDR = 0x10;

const uint8_t register_RX_PW_P0 = 0x11;
const uint8_t register_RX_PW_P1 = 0x12;
const uint8_t register_RX_PW_P2 = 0x13;
const uint8_t register_RX_PW_P3 = 0x14;
const uint8_t register_RX_PW_P4 = 0x15;
const uint8_t register_RX_PW_P5 = 0x16;

const uint8_t register_FIFO_STATUS = 0x17;

const uint8_t MASK_RX_DR = 1<<6;
const uint8_t MASK_TX_DS = 1<<5;
const uint8_t MASK_MAX_RT = 1<<4;
const uint8_t EN_CRC = 1<<3;
const uint8_t CRCO = 1<<2;
const uint8_t PWR_UP = 1<<1;
const uint8_t PRIM_RX = 1<<0;

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
