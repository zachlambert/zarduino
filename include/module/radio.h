#ifndef ZARDUINO_MODULES_RADIO
#define ZARDUINO_MODULES_RADIO

// nRF24L01 module

#include "core/pins.h"

typedef enum {
    RADIO_CONFIG_CRC_1BYTE,
    RADIO_CONFIG_CRC_2BYTES
} RadioConfigCrc;

typedef enum {
    RADIO_CONFIG_ADDRESS_WIDTH_3 = 0b01,
    RADIO_CONFIG_ADDRESS_WIDTH_4 = 0b10,
    RADIO_CONFIG_ADDRESS_WIDTH_5 = 0b11
} RadioConfigAddressWidth;

typedef enum {
    RADIO_CONFIG_AIR_DATA_RATE_1MBPS,
    RADIO_CONFIG_AIR_DATA_RATE_2MBPS,
} RadioConfigAirDataRate;

typedef enum {
    RADIO_CONFIG_OUTPUT_POWER_NEG18DB,
    RADIO_CONFIG_OUTPUT_POWER_NEG12DB,
    RADIO_CONFIG_OUTPUT_POWER_NEG6DB,
    RADIO_CONFIG_OUTPUT_POWER_0DB
} RadioConfigOutputPower;

typedef struct {
    uint64_t address;
    uint8_t payload_size;
    uint8_t en : 1;
} RadioPipeConfig;

typedef struct {
    uint8_t interrupt_en_rx : 1;
    uint8_t interrupt_en_tx_ack : 1;
    uint8_t interrupt_en_max_retransmit : 1;
    uint8_t en_crc : 1;
    RadioConfigCrc crc;
    RadioConfigAddressWidth address_width;
    uint8_t auto_retransmit_delay;
    uint8_t auto_retransmit_count;
    uint8_t frequency_channel;
    RadioConfigAirDataRate air_data_rate;
    RadioConfigOutputPower output_power;

    // Only RX pipes 1-5 used for rx
    uint32_t rx_base_address;
    uint8_t rx_pipe_addresses[5];
    uint8_t rx_pipe_enable[5];
    uint8_t rx_payload_sizes[5];

    // Pipe RX 0 and TX have the same address
    uint64_t tx_address;
    // RX 0 is also automatically given a suitable
    // payload size

    Pin CE;
    Pin CSN;
    Pin IRQ;
} RadioConfig;

RadioConfig radio_create_config(void);

void radio_init_as_receiver(RadioConfig *config);
void radio_init_as_transmitter(RadioConfig *config);

uint8_t radio_read_rx(RadioConfig *config);
void radio_write_tx(RadioConfig *config, uint8_t value);

void radio_start(RadioConfig *config);
void radio_stop(RadioConfig *config);

#endif
