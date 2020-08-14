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

    // Use the same default values as those used by
    // the device, except for payload size of the default
    // enabled RX pipes

    config.interrupt_en_rx = 1;
    config.interrupt_en_tx_ack = 1;
    config.interrupt_en_max_retransmit = 1;
    config.en_crc = 1;
    config.crc = RADIO_CONFIG_CRC_1BYTE;
    config.address_width = RADIO_CONFIG_ADDRESS_WIDTH_5;
    config.auto_retransmit_delay = 0;
    config.auto_retransmit_count = 3;
    config.frequency_channel = 2;
    config.air_data_rate = RADIO_CONFIG_AIR_DATA_RATE_2MBPS;
    config.output_power = RADIO_CONFIG_OUTPUT_POWER_0DB;

    config.rx_config[0].address = 0xE7E7E7E7E7;
    config.rx_config[0].payload_size = 1;
    config.rx_config[0].en = 1;

    config.rx_config[1].address = 0xC2C2C2C2C2;
    config.rx_config[1].payload_size = 1;
    config.rx_config[1].en = 1;

    config.rx_config[2].address = 0xC3;
    config.rx_config[3].address = 0xC4;
    config.rx_config[4].address = 0xC5;
    config.rx_config[5].address = 0xC6;

    config.tx_address = 0xE7E7E7E7E7;

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

    // Ensure the radio is in standby/power down mode
    // by ensuring CE is low. Should be 0 anyway.
    gpio_write(config->CE, 0);

    // 0x00 CONFIG
    uint8_t config_reg = 0;
    config_reg |= config->interrupt_en_rx << MASK_RX_DR;
    config_reg |= config->interrupt_en_tx_ack << MASK_TX_DS;
    config_reg |= config->interrupt_en_max_retransmit << MASK_MAX_RT;
    config_reg |= config->en_crc << EN_CRC;
    config_reg |= config->crc << CRCO;
    radio_register_write(config, register_CONFIG, config_reg);
    
    // 0x02 EN_RXADDR
    uint8_t en_rxaddr_reg = 0;
    for (size_t i = 0; i < 6; i++) {
        en_rxaddr_reg |= config->rx_config[i].en << i;
    }
    radio_register_write(config, register_EN_RXADDR, en_rxaddr_reg);

    // 0x03 SETUP_AW
    uint8_t setup_aw_reg = (config->address_width & AW_mask) << AW_shift;
    radio_register_write(
        config,
        register_SETUP_AW,
        setup_aw_reg
    );

    // 0x04 SETUP_RETR
    uint8_t setup_retr_reg = 0;
    setup_retr_reg |= (config->auto_retransmit_delay & ARD_mask) << ARD_shift;
    setup_retr_reg |= (config->auto_retransmit_count & ARC_mask) << ARC_shift;

    radio_register_write(config, register_SETUP_RETR, setup_retr_reg);

    // 0x05 RF_CH
    uint8_t rf_ch_reg = (config->frequency_channel & RF_CH_mask) << RF_CH_shift;
    radio_register_write(config, register_RF_CH, rf_ch_reg);

    // 0x06 RF_SETUP
    uint8_t rf_setup_reg = 0;
    rf_setup_reg |= config->air_data_rate << RF_DR;

    // 0x0A -> 0x0F RX_ADDR_P0 -> RX_ADDR_P5
    // TODO Need multiple byte write
    
    // 0x10 TX_ADDR
    // TODO Need multiple byte write

    // 0x11 -> 0x16 RX_PW_P0 -> RX_RW_P5
    for (size_t i = 0; i < 6; i++) {
        radio_register_write(
            config,
            register_RX_PW_P0 + i,
            (config->rx_config[i].payload_size & RX_PW_P0_mask) << RX_PW_P0_shift
        );
    }

    // Reset the status register
    radio_reset_status(config);

    // Flush buffers
    spi_transfer_byte(config->CSN, command_FLUSH_RX);
    spi_transfer_byte(config->CSN, command_FLUSH_TX);

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
