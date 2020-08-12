#include "module/radio.h"
#include "comms/spi.h"
#include "core/pins.h"
#include "comms/uart.h"

#include "module/radio_constants.h"


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
    uint8_t config_value = 
        radio_register_read(config, register_CONFIG);
    config_value |= 1 << PRIM_RX;
    config_value |= 1 << PWR_UP;
    radio_register_write(config, register_CONFIG, config_value);

    // Setup RX pipe
    // Default: 0 and 1 enabled
    // Leave default addresses
    // Need to set the number of bytes in the RX payload
    uint8_t num_bytes = 1;
    radio_register_write(
        config,
        register_RX_PW_P0,
        (num_bytes & RX_PW_P0_mask) << RX_PW_P0_shift
    );
}

void radio_set_mode_tx(RadioConfig *config)
{
    uint8_t config_value =
        radio_register_read(config, register_CONFIG);
    config_value &= ~(1 << PRIM_RX);
    config_value |= 1 << PWR_UP;
    radio_register_write(config, register_CONFIG, config_value);

    // Setup TX
    // Leave address default -> RX pipe 0 default address
}

void radio_start(RadioConfig *config)
{
    gpio_write(config->CE, 1);
}

void radio_stop(RadioConfig *config)
{
    gpio_write(config->CE, 0);
}

uint8_t read_rx_byte(RadioConfig *config)
{
    uint8_t data_in[] = {command_R_RX_PAYLOAD, command_NOP};
    uint8_t data_out[2];
    spi_transfer_bytes(config->CSN, data_in, data_out, 2);
    return data_out[1];
}

void write_tx_byte(RadioConfig *config, uint8_t value)
{
    uint8_t data_in[] = {command_W_TX_PAYLOAD, value};
    spi_transfer_bytes(config->CSN, data_in, 0, 2);
}
