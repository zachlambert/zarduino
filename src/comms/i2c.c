#include "zarduino/comms/i2c.h"
#include "zarduino/core/regs.h"
#include "zarduino/core/pins.h"
#include <avr/io.h>

// Status codes for master mode
// These cover the whole TWSR register, but with the
// first two bits (the prescaler bits) masked.
// Note: SLA = slave address
const uint8_t STATUS_START = 0x08;
const uint8_t STATUS_REPEATED_START = 0x10;
const uint8_t STATUS_SLA_W_ACK = 0x18;
const uint8_t STATUS_SLA_W_NACK = 0x20;
const uint8_t STATUS_DATA_W_ACK = 0x28;
const uint8_t STATUS_DATA_W_NACK = 0x30;
const uint8_t STATUS_ARBITRATION_LOST = 0x38;
const uint8_t STATUS_SLA_R_ACK = 0x40;
const uint8_t STATUS_SLA_R_NACK = 0x48;
const uint8_t STATUS_DATA_R_ACK = 0x50;
const uint8_t STATUS_DATA_R_NACK = 0x58;

#define I2C_FREQ 100000L
I2CConfig i2c_create_config(void)
{
    I2CConfig config = {};
    // Default: Get a 100kHZ I2C clock
    config.bit_rate_prescaler = I2C_BIT_RATE_PRESCALER_1;
    config.bit_rate_reduction = ((F_CPU / I2C_FREQ) - 16) / 2;
    return config;
}

void i2c_init_master(I2CConfig *config)
{
    TWBR = config->bit_rate_reduction;
    reg_write_mask(&TWSR, TWPS0, 0b11, config->bit_rate_prescaler);
    // Enable pullps on SDA and SCL
    gpio_mode_input_pullup(PIN_SDA);
    gpio_mode_input_pullup(PIN_SCL);

    reg_write_bit(&TWCR, TWEN, 1);
}

void i2c_send_start(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
}

void i2c_send_repeated_start(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
}

void i2c_send_slave_address(uint8_t i2c_address, uint8_t read)
{
    TWDR = (i2c_address<<1) | (read & 0x01);
    TWCR = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
}

void i2c_send_data(uint8_t data)
{
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
}

uint8_t i2c_receive_data(uint8_t en_ack)
{
    TWCR = (1<<TWINT) | (1<<TWEN) | (en_ack<<TWEA);
    while(!(TWCR & (1<<TWINT)));
    return TWDR;
}

void i2c_send_stop(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

void i2c_read_register(
    uint8_t i2c_address,
    uint8_t reg_address,
    uint8_t *data_out,
    size_t num_bytes)
{
    i2c_send_start();
    i2c_send_slave_address(i2c_address, 0);
    i2c_send_data(reg_address);
    i2c_send_repeated_start();
    i2c_send_slave_address(i2c_address, 1);
    for (size_t i = 0; i < num_bytes; i++) {
        if (i < num_bytes-1)
            data_out[i] = i2c_receive_data(1);
        else
            data_out[i] = i2c_receive_data(0);
    }
    i2c_send_stop();
}

void i2c_write_register(
    uint8_t i2c_address,
    uint8_t reg_address,
    uint8_t *data_in,
    size_t num_bytes)
{
    i2c_send_start();
    i2c_send_slave_address(i2c_address, 0);
    i2c_send_data(reg_address);
    for (size_t i = 0; i < num_bytes; i++) {
        i2c_send_data(data_in[i]);
    }
    i2c_send_stop();
}


void i2c_write(uint8_t i2c_address, uint8_t *data_in, size_t num_bytes)
{
    i2c_send_start();
    i2c_send_slave_address(i2c_address, 0);
    for (size_t i = 0; i < num_bytes; i++) {
        i2c_send_data(data_in[i]);
    }
    i2c_send_stop();
}
