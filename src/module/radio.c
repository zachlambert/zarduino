#include "zarduino/module/radio.h"
#include "zarduino/comms/spi.h"
#include "zarduino/core/pins.h"
#include "zarduino/comms/uart.h"

#include "module/radio_constants.h"


void radio_register_read(RadioConfig *config, uint8_t address, uint8_t *data_out, size_t num_bytes)
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

void radio_register_write(RadioConfig *config, uint8_t address, uint8_t *data_in, size_t num_bytes)
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
    uint8_t STATUS = 0;
    reg_write_bit(&STATUS, RX_DR, 1);
    reg_write_bit(&STATUS, TX_DS, 1);
    reg_write_bit(&STATUS, MAX_RT, 1);
    radio_register_write(config, STATUS_address, &STATUS, 1);
}

void radio_init_common(RadioConfig *config)
{
    // === GPIO ===

    gpio_mode_output(config->CE);
    gpio_mode_output(config->CSN);
    if (config->IRQ)
        gpio_mode_input(config->IRQ);
    gpio_write(config->CSN, 1);
    gpio_write(config->CE, 0);
    // Ensure CE is low, so its in power down / standby mode

    // === SPI ====
    // Initialised by main program

    // === CONFIG register ===

    uint8_t CONFIG = 0;
    reg_write_bit(&CONFIG, MASK_RX_DR, config->interrupt_en_rx);
    reg_write_bit(&CONFIG, MASK_TX_DS, config->interrupt_en_tx_ack);
    reg_write_bit(&CONFIG, MASK_MAX_RT, config->interrupt_en_max_retransmit);
    reg_write_bit(&CONFIG, EN_CRC, config->en_crc);
    reg_write_bit(&CONFIG, CRCO, config->crc);
    radio_register_write(config, CONFIG_address, &CONFIG, 1);

    // 0x02 EN_RXADDR
    uint8_t EN_RXADDR = 0;
    reg_write_bit(&EN_RXADDR, 0, 1); // Always enable pipe 0
    for (size_t i = 0; i < 5; i++) {
        reg_write_bit(&EN_RXADDR, i+1, config->rx_pipe_enable[i]);
    }
    radio_register_write(config, EN_RXADDR_address, &EN_RXADDR, 1);

    // 0x03 SETUP_AW
    uint8_t SETUP_AW = 0;
    reg_write_mask(&SETUP_AW, AW_shift, AW_mask, config->address_width);
    radio_register_write(config, SETUP_AW_address, &SETUP_AW, 1);

    // 0x04 SETUP_RETR
    uint8_t SETUP_RETR = 0;
    reg_write_mask(&SETUP_RETR, ARD_shift, ARD_mask, config->auto_retransmit_delay);
    reg_write_mask(&SETUP_RETR, ARC_shift, ARC_mask, config->auto_retransmit_count);
    radio_register_write(config, SETUP_RETR_address, &SETUP_RETR, 1);

    // 0x05 RF_CH
    uint8_t RF_CH = 0;
    reg_write_mask(&RF_CH, RF_CH_shift, RF_CH_mask, config->frequency_channel);
    radio_register_write(config, RF_CH_address, &RF_CH, 1);

    // 0x06 RF_SETUP
    uint8_t RF_SETUP = 0;
    reg_write_bit(&RF_SETUP, RF_DR, config->air_data_rate);
    radio_register_write(config, RF_SETUP_address, &RF_SETUP, 1);

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

    uint8_t TX_ADDR[5];
    for (size_t i = 0; i < 5; i++) {
        if (i < 2 + config->address_width)
            TX_ADDR[i] = config->tx_address >> 8*i;
        else
            TX_ADDR[i] = 0;
    }
    radio_register_write(config, TX_ADDR_address, TX_ADDR, 5);
    radio_register_write(config, RX_ADDR_P0_address, TX_ADDR, 5);

    // === RX ADDRESSES ===

    uint8_t RX_ADDR_P1[5];
    RX_ADDR_P1[0] = config->rx_pipe_addresses[0];
    for (size_t i = 1; i < 5; i++) {
        if (i < 2 + config->address_width)
            RX_ADDR_P1[i] = config->rx_base_address >> 8*(i-1);
        else
            RX_ADDR_P1[i] = 0;
    }
    radio_register_write(config, RX_ADDR_P1_address, RX_ADDR_P1, 5);

    // Register addresses are consecutive, so use a loop instead of writing
    // each out manually
    for (size_t i = 1; i < 5; i++) {
        radio_register_write(
            config,
            RX_ADDR_P1_address + i,
            &config->rx_pipe_addresses[i],
            1
        );
    }

    // === RX PIPE PAYLOAD SIZES ===

    // Assume payload size 1 for RX pipe 0
    uint8_t rx0_payload_size = 1;
    radio_register_write(config, RX_PW_P0_address, &rx0_payload_size, 1);
    for (size_t i = 1; i < 6; i++) {
        radio_register_write(
            config,
            RX_PW_P0_address+i,
            &config->rx_payload_sizes[i-1],
            1
        );
    }

    // === RESET STATUS AND FLUSH BUFFERS ===

    radio_reset_status(config);
    spi_transfer_byte(config->CSN, command_FLUSH_RX);
    spi_transfer_byte(config->CSN, command_FLUSH_TX);

    // === Power up ===

    reg_write_bit(&CONFIG, PWR_UP, 1);
    radio_register_write(config, CONFIG_address, &CONFIG, 1);
}

void radio_init_as_receiver(RadioConfig *config)
{
    radio_init_common(config);

    uint8_t CONFIG;
    radio_register_read(config, CONFIG_address, &CONFIG, 1);
    reg_write_bit(&CONFIG, PRIM_RX, 1);
    radio_register_write(config, CONFIG_address, &CONFIG, 1);

}

void radio_init_as_transmitter(RadioConfig *config)
{
    radio_init_common(config);

    uint8_t CONFIG;
    radio_register_read(config, CONFIG_address, &CONFIG, 1);
    reg_write_bit(&CONFIG, PRIM_RX, 0);
    radio_register_write(config, CONFIG_address, &CONFIG, 1);
}

void radio_start(RadioConfig *config)
{
    gpio_write(config->CE, 1);
}

void radio_stop(RadioConfig *config)
{
    gpio_write(config->CE, 0);
}

RadioRxStatus radio_read_rx(RadioConfig *config, uint8_t *data_out, size_t num_bytes)
{
    uint8_t spi_data_in[1+num_bytes];
    spi_data_in[0] = command_R_RX_PAYLOAD;
    for (size_t i = 0; i < num_bytes; i++) {
        spi_data_in[i+1] = command_NOP;
    }
    uint8_t spi_data_out[1+num_bytes];
    spi_transfer_bytes(config->CSN, spi_data_in, spi_data_out, 1+num_bytes);
    for (size_t i = 0; i < num_bytes; i++) {
        data_out[i] = spi_data_out[i+1];
    }
    spi_transfer_byte(config->CSN, command_FLUSH_RX);
    return reg_read_mask(
        &spi_data_out[0], RX_P_NO_shift, RX_P_NO_mask
    );
}

void radio_write_tx(RadioConfig *config, uint8_t *data_in, size_t num_bytes)
{
    // spi_transfer_byte(config->CSN, command_FLUSH_TX);
    uint8_t spi_data_in[1+num_bytes];
    spi_data_in[0] = command_W_TX_PAYLOAD;
    for (size_t i = 0; i < num_bytes; i++) {
        spi_data_in[i+1] = data_in[i];
    }
    spi_transfer_bytes(config->CSN, spi_data_in, 0, 1+num_bytes);
}
