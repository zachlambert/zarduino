#ifndef ZARDUINO_MODULES_RADIO
#define ZARDUINO_MODULES_RADIO

// nRF24L01 module

#include "core/pins.h"

typedef enum {
    RADIO_CONFIG_CRC_1BYTE,
    RADIO_CONFIG_CRC_2BYTES
} RadioConfigCrc;

typedef struct {
    uint8_t interrupt_en_rx : 1;
    uint8_t interrupt_en_tx_ack : 1;
    uint8_t interrupt_en_max_retransmit : 1;
    uint8_t en_crc : 1;
    RadioConfigCrc crc;
    Pin CE;
    Pin CSN;
    Pin IRQ;
} RadioConfig;

RadioConfig radio_create_config(void);

void radio_init(RadioConfig *config);
void radio_set_mode_rx(RadioConfig *config);
void radio_set_mode_tx(RadioConfig *config);

uint8_t radio_register_read(
    RadioConfig *config,
    uint8_t register_address
);

void radio_register_write(
    RadioConfig *config,
    uint8_t register_address,
    uint8_t value
);

uint8_t radio_read_rx(RadioConfig *config);
void radio_write_tx(RadioConfig *config, uint8_t value);

void radio_start(RadioConfig *config);
void radio_stop(RadioConfig *config);

#endif
