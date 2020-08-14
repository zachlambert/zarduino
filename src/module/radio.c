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

void radio_register_read_bytes(RadioConfig *config, uint8_t address, uint8_t *data_out, size_t num_bytes)
{
    uint8_t spi_data_in[num_bytes+1];
    spi_data_in[0] = command_R_REGISTER(address);
    for (size_t i = 0; i < num_bytes; i++) {
        spi_data_in[i+1] = command_NOP;
    }
    uint8_t spi_data_out[num_bytes+1];
    spi_transfer_bytes(config->CSN, spi_data_in, spi_data_out, num_bytes+1);
    for (size_t i = 0; i < num_bytes; i++) {
        data_out[i] = spi_data_out[i+1];
    }
}

void radio_register_write_bytes(RadioConfig *config, uint8_t address, uint8_t *data_in, size_t num_bytes)
{
    uint8_t spi_data_in[num_bytes+1];
    spi_data_in[0] = command_W_REGISTER(address);
    for (size_t i = 0; i < num_bytes; i++) {
        spi_data_in[i+1] = data_in[i];
    }
    spi_transfer_bytes(config->CSN, spi_data_in, 0, num_bytes+1);
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

    config.tx_address = 0xE7E7E7E7E7;

    config.rx_base_address = 0xC2C2C2C2;
    config.rx_pipe_addresses[0] = 0xC2;
    config.rx_pipe_addresses[1] = 0xC3;
    config.rx_pipe_addresses[2] = 0xC4;
    config.rx_pipe_addresses[3] = 0xC5;
    config.rx_pipe_addresses[4] = 0xC6;
    config.rx_pipe_enable[0] = 1;

    config.rx_payload_sizes[0] = 1;

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
    uint8_t en_rxaddr_reg = 1; // Always enable RX pipe 0
    // Enable the dedicated RX pipes
    for (size_t i = 0; i < 5; i++) {
        en_rxaddr_reg |= config->rx_pipe_enable[i] << (i+1);
    }
    printf("Enabled rx pipes: %x\n", en_rxaddr_reg);
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

    // Addresses
    // - All RX pipes, and TX have 3-5 byte addresses,
    //   depending on the value of address_width
    // - Only RX pipes 0 and 1, and TX have these written
    //   to their registers, which are 5 bytes wide.
    // - RX pipes 2 - 5 have 1 byte registers, which set
    //   the LSB of their address, and share the MSBs with
    //   pipe 1.
    // - Hence, the upper bytes of pipe 1 address are a base
    //   address for pipes 1-5.
    //
    // - When in TX mode, RX pipe 0 is used to receive the
    //   acknowledgement packets. Therefore, it needs to have
    //   the same address.
    // - To simplify things, RX is always paired with TX and
    //   given the same address, even if TX is not used.


    // === TX ADDRESS ===

    uint8_t tx_addr_data[2+config->address_width];
    for (size_t i = 0; i < 2+config->address_width; i++) {
        tx_addr_data[i] = config->tx_address >> 8*i;
    }

    radio_register_write_bytes(
        config,
        register_RX_ADDR_P0,
        tx_addr_data, // RX 0 and TX have same address
        2+config->address_width
    );

    radio_register_write_bytes(
        config,
        register_TX_ADDR,
        tx_addr_data,
        2+config->address_width
    );

    for (size_t i = 0; i < 5; i++) printf("%x ", tx_addr_data[i]);
    printf("\n");

    // === RX ADDRESSES ===

    uint8_t rx1_addr_data[2+config->address_width];
    rx1_addr_data[0] = config->rx_pipe_addresses[0];
    for (size_t i = 0; i < 1+config->address_width; i++) {
        rx1_addr_data[i+1] = config->rx_base_address >> 8*i;
    }

    for (size_t i = 0; i < 5; i++) printf("%x ", rx1_addr_data[i]);
    printf("\n");

    radio_register_write_bytes(
        config,
        register_RX_ADDR_P1,
        rx1_addr_data,
        2+config->address_width
    );

    // for (size_t i = 1; i < 5; i++) {
    //     radio_register_write(
    //         config,
    //         register_RX_ADDR_P2 + i,
    //         config->rx_pipe_addresses[i]
    //     );
    // }

    // 0x11 RX_PW_P0
    radio_register_write(
        config,
        register_RX_PW_P0,
        1 // Assume a 1 byte payload is fine for RX 0
          // which is receives the TX acknowledgment packet
    );

    // 0x12 -> 0x16 RX_PW_P1 -> RX_RW_P5
    for (size_t i = 0; i < 5; i++) {
        radio_register_write(
            config,
            register_RX_PW_P1 + i,
            1//(config->rx_payload_sizes[i] & RX_PW_P0_mask) << RX_PW_P0_shift
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
    uint8_t reg_config =
        radio_register_read(config, register_CONFIG);
    reg_config |= 1 << PRIM_RX;
    radio_register_write(config, register_CONFIG, reg_config);

}

void radio_set_mode_tx(RadioConfig *config)
{
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
