
#include "comms/uart.h"
#include "comms/i2c.h"
#include "core/regs.h"
#include <avr/interrupt.h>

// Status codes for master mode
// Note: SLA = slave address
const uint8_t STATUS_START = 0x08; //b00001000
const uint8_t STATUS_REPEATED_START = 0x10; // 0b00001010
const uint8_t STATUS_SLA_W_ACK = 0x18; // 0b00010010
const uint8_t STATUS_SLA_W_NACK = 0x20; // 0b00010100
const uint8_t STATUS_DATA_W_ACK = 0x28; // 0b00011100
const uint8_t STATUS_DATA_W_NACK = 0x30; // 0b00011110
const uint8_t STATUS_ARBITRATION_LOST = 0x38; // 0b00100110
const uint8_t STATUS_SLA_R_ACK = 0x40; // 0b00101000
const uint8_t STATUS_SLA_R_NACK = 0x48; // 0b00110000
const uint8_t STATUS_DATA_R_ACK = 0x50; // 0b00110010
const uint8_t STATUS_DATA_R_NACK = 0x58; // 0b00111010

I2CConfig i2c_create_config(void)
{
    I2CConfig config = {};
    config.bit_rate_reduction = 0;
    config.bit_rate_prescaler = I2C_BIT_RATE_PRESCALER_1;
    return config;
}

void i2c_init_master(I2CConfig *config)
{
    TWBR = config->bit_rate_reduction;
    reg_write_mask(&TWSR, TWPS0, 0b11, config->bit_rate_prescaler);
}

void i2c_send_start(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
    // Status should be STATUS_START
}

void i2c_send_repeated_start(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
    // Status should be STATUS_REPEATED_START
}

void i2c_send_slave_address(uint8_t i2c_address, uint8_t read)
{
    TWDR = (i2c_address<<1) | (read & 0x01);
    TWCR = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
    // STATUS should be one of:
    // STATUS_SLA_W_ACK, STATUS_SLA_W_NACK (if read=0)
    // STATUS_SLA_R_ACK, STATUS_SLA_R_NACK (if read=1)
}

void i2c_send_data(uint8_t data)
{
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
    // Status should be one of:
    // STATUS_DATA_W_ACK or STATUS_DATA_W_NACK
}

uint8_t i2c_receive_data(void)
{
    TWCR = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
    // Status should be one of:
    // STATUS_DATA_W_ACK or STATUS_DATA_W_NACK
    return TWDR; // Data has been read into TWDR
}

void i2c_send_stop(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

void i2c_read(uint8_t i2c_address, uint8_t *data_out, size_t num_bytes)
{
    i2c_send_start();
    i2c_send_slave_address(i2c_address, 0);
    for (size_t i = 0; i < num_bytes; i++) {
        data_out[i] = i2c_receive_data();
        if (i < num_bytes-1)
            i2c_send_repeated_start();
    }
    i2c_send_stop();
}

void i2c_write(uint8_t i2c_address, uint8_t *data_in, size_t num_bytes)
{
    i2c_send_start();
    i2c_send_slave_address(i2c_address, 0);
    for (size_t i = 0; i < num_bytes; i++) {
        i2c_send_data(data_in[i]);
        if (i < num_bytes-1)
            i2c_send_repeated_start();
    }
    i2c_send_stop();
}

