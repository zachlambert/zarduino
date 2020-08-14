#include "module/radio.h"
#include "comms/spi.h"
#include "core/pins.h"
#include "comms/uart.h"

#include "module/radio_constants.h"


uint8_t radio_register_read(RadioConfig *config, uint8_t address)
{
    uint8_t data_in[] = {
        command_R_REGISTER(address),
        command_NOP
    };
    uint8_t data_out[2];
    spi_transfer_bytes(config->CSN, data_in, data_out, 2);
    return data_out[1];
}

void radio_register_write(RadioConfig *config, uint8_t address, uint8_t value)
{
    uint8_t data_in[] = {
        command_W_REGISTER(address),
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

void radio_reset_status(RadioConfig *config)
{
    radio_register_write(
        config,
        register_STATUS,
        1<<RX_DR | 1<<TX_DS | 1<<MAX_RT
    );
}

void radio_init(RadioConfig *config)
{
    // GPIO
    gpio_mode_output(config->CE);
    gpio_mode_output(config->CSN);
    if (config->IRQ)
        gpio_mode_input(config->IRQ);
    gpio_write(config->CSN, 1);

    // SPI
    // Used over many modules, so the main program should
    // handle SPI initialisation when its needed.

    // Set retries
    // uint8_t ard_value = 5;
    // uint8_t arc_value = 15;
    // radio_register_write(
    //     config,
    //     register_SETUP_RETR,
    //     (ard_value & ARD_mask) << ARD_shift |
    //         (arc_value & ARC_mask) << ARC_shift
    // );

    // // Set data rate to 1MBPS
    // uint8_t rf_setup = radio_register_read(
    //     config, register_RF_SETUP
    // );
    // rf_setup &= ~(1<<3); // 1MBps instead of default 2
    // radio_register_write(config, register_RF_SETUP, rf_setup);

    // Reset the status register
    radio_reset_status(config);

    // Set channel

    // uint8_t channel_freq = 1;
    // radio_register_write(
    //     config,
    //     register_RF_CH,
    //     (channel_freq & RF_CH_mask) << RF_CH_shift
    // );

    // Flush buffers
    spi_transfer_byte(config->CSN, command_FLUSH_RX);
    spi_transfer_byte(config->CSN, command_FLUSH_TX);

    // Clear config
    radio_register_write(
        config,
        register_CONFIG,
        1 << EN_CRC | 1 << CRCO
    );

    // Power up
    uint8_t reg_config = radio_register_read(config, register_CONFIG);
    reg_config |= 1<<PWR_UP;
    radio_register_write(config, register_CONFIG, reg_config);
}

void radio_set_mode_rx(RadioConfig *config)
{
    // 1. Set the pipe address
    // Leave default

    // 2. Set the payload size
    uint8_t num_bytes = 1;
    radio_register_write(
        config,
        register_RX_PW_P0,
        (num_bytes & RX_PW_P0_mask) << RX_PW_P0_shift
    );

    // 3. Enable the pipe (already done by default for 0, 1)
    // uint8_t reg_en_rxaddr = radio_register_read(
    //     config, register_EN_RXADDR
    // );
    // reg_en_rxaddr |= 1 << ERX_P0;
    // radio_register_write(
    //     config,
    //     register_EN_RXADDR,
    //     reg_en_rxaddr
    // );

    // 4. Set PRIM_RX to go into RX mode
    uint8_t reg_config =
        radio_register_read(config, register_CONFIG);
    reg_config |= 1 << PRIM_RX;
    radio_register_write(config, register_CONFIG, reg_config);

}

void radio_set_mode_tx(RadioConfig *config)
{
    // 1. Set address
    // Leave address default -> RX pipe 0 default address

    // 2. Also need to use the rx pipe 0, so set the
    // address
    // Again, leave default

    // 3. Set payload size for pipe 0
    uint8_t num_bytes = 1;
    radio_register_write(
        config,
        register_RX_PW_P0,
        (num_bytes & RX_PW_P0_mask) << RX_PW_P0_shift
    );

    // 4. Set PRIM_RX low
    uint8_t reg_config =
        radio_register_read(config, register_CONFIG);
    reg_config &= ~(1 << PRIM_RX);
    radio_register_write(config, register_CONFIG, reg_config);
}

void radio_start(RadioConfig *config)
{
    // 2. Set CE high
    gpio_write(config->CE, 1);
}

void radio_stop(RadioConfig *config)
{
    gpio_write(config->CE, 0);
}

uint8_t radio_read_rx(RadioConfig *config)
{
    uint8_t data_in[] = {command_R_RX_PAYLOAD, command_NOP};
    uint8_t data_out[2];
    spi_transfer_bytes(config->CSN, data_in, data_out, 2);
    // Then need to flush rx buffer
    spi_transfer_byte(config->CSN, command_FLUSH_RX);
    return data_out[1];
}

void radio_write_tx(RadioConfig *config, uint8_t value)
{
    // Flush the TX buffer
    spi_transfer_byte(config->CSN, command_FLUSH_TX);
    uint8_t data_in[] = {command_W_TX_PAYLOAD, value};
    spi_transfer_bytes(config->CSN, data_in, 0, 2);
}
