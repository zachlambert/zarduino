#ifndef ZARDUINO_MODULE_RADIO_CONSTANTS
#define ZARDUINO_MODULE_RADIO_CONSTANTS

#include <stdint.h>

// === Commands ===

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


// === Registers ===

#define CONFIG_address 0x00
const uint8_t EN_AA_address = 0x01;
const uint8_t EN_RXADDR_address = 0x02;
const uint8_t SETUP_AW_address = 0x03;
const uint8_t SETUP_RETR_address = 0x04;
const uint8_t RF_CH_address = 0x05;
const uint8_t RF_SETUP_address = 0x06;
const uint8_t STATUS_address = 0x07;
const uint8_t OBSERVE_TX_address = 0x08;
const uint8_t CD_address = 0x09;

const uint8_t RX_ADDR_P0_address = 0x0A;
const uint8_t RX_ADDR_P1_address = 0x0B;
const uint8_t RX_ADDR_P2_address = 0x0C;
const uint8_t RX_ADDR_P3_address = 0x0D;
const uint8_t RX_ADDR_P4_address = 0x0E;
const uint8_t RX_ADDR_P5_address = 0x0F;
const uint8_t TX_ADDR_address = 0x10;

const uint8_t RX_PW_P0_address = 0x11;
const uint8_t RX_PW_P1_address = 0x12;
const uint8_t RX_PW_P2_address = 0x13;
const uint8_t RX_PW_P3_address = 0x14;
const uint8_t RX_PW_P4_address = 0x15;
const uint8_t RX_PW_P5_address = 0x16;

const uint8_t FIFO_STATUS_address = 0x17;


// === Bits ===

// register_CONFIG
const uint8_t MASK_RX_DR = 6;
const uint8_t MASK_TX_DS = 5;
const uint8_t MASK_MAX_RT = 4;
const uint8_t EN_CRC = 3;
const uint8_t CRCO = 2;
const uint8_t PWR_UP = 1;
const uint8_t PRIM_RX = 0;

// register_EN_AA
const uint8_t ENAA_P5 = 5;
const uint8_t ENAA_P4 = 4;
const uint8_t ENAA_P3 = 3;
const uint8_t ENAA_P2 = 2;
const uint8_t ENAA_P1 = 1;
const uint8_t ENAA_P0 = 0;

// register_EN_RXADDR
const uint8_t ERX_P5 = 5;
const uint8_t ERX_P4 = 4;
const uint8_t ERX_P3 = 3;
const uint8_t ERX_P2 = 2;
const uint8_t ERX_P1 = 1;
const uint8_t ERX_P0 = 0;

// register_SETUP_AW
const uint8_t AW_shift = 0;
const uint8_t AW_mask = 0b11;

// register_SETUP_AW
const uint8_t ARD_shift = 4;
const uint8_t ARD_mask = 0x0F;
const uint8_t ARC_shift = 0;
const uint8_t ARC_mask = 0x0F;

// register_RF_CH
const uint8_t RF_CH_shift = 0;
const uint8_t RF_CH_mask = 0x7F;

// register_RF_SETUP
const uint8_t PLL_LOCK = 4;
const uint8_t RF_DR = 3;
const uint8_t RF_PWR_shift = 1;
const uint8_t RF_PWR_mask = 0b11;
const uint8_t LNA_HCURR = 0;

// register_STATUS
const uint8_t RX_DR = 6;
const uint8_t TX_DS = 5;
const uint8_t MAX_RT = 4;
const uint8_t RX_P_NO_shift = 1;
const uint8_t RX_P_NO_mask = 0b111;
const uint8_t TX_FULL_rSTATUS = 0;

// register OBSERVE_TX
const uint8_t PLOS_CNT_shift = 4;
const uint8_t PLOS_CNT_mask = 0x0F;
const uint8_t ARC_CNT_shit = 0;
const uint8_t ARC_CNT_mask = 0x0F;

// register RX_PW_Pn
const uint8_t RX_PW_Pn_shift = 0;
const uint8_t RX_PW_Pn_mask = 0b00111111;

// register_FIFO_STATUS
const uint8_t TX_REUSE = 6;
const uint8_t TX_FULL_rFIFO_STATUS = 5;
const uint8_t TX_EMPTY = 4;
const uint8_t RX_FULL = 1;
const uint8_t RX_EMPTY = 0;

#endif
