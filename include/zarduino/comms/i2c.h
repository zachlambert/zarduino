#ifndef ZARDUINO_COMMS_I2C_H
#define ZARDUINO_COMMS_I2C_H

#include <stdint.h>
#include <stdlib.h>

typedef enum {
    I2C_BIT_RATE_PRESCALER_1,
    I2C_BIT_RATE_PRESCALER_4,
    I2C_BIT_RATE_PRESCALER_16,
    I2C_BIT_RATE_PRESCALER_64,
} I2CBitRatePrescaler;

typedef struct {
    uint8_t bit_rate_reduction;
    I2CBitRatePrescaler bit_rate_prescaler;
} I2CConfig;

I2CConfig i2c_create_config(void);
void i2c_init_master(I2CConfig *config);

void i2c_read_register(
    uint8_t i2c_address,
    uint8_t reg_address,
    uint8_t *data_out,
    size_t num_bytes
);

void i2c_write_register(
    uint8_t i2c_address,
    uint8_t reg_address,
    uint8_t *data_in,
    size_t num_bytes
);

void i2c_write(uint8_t i2c_address, uint8_t *data_in, size_t num_bytes);

// For modules which need more control over the process
void i2c_send_start(void);
void i2c_send_repeated_start(void);
void i2c_send_slave_address(uint8_t i2c_address, uint8_t read);
void i2c_send_data(uint8_t data);
uint8_t i2c_receive_data(uint8_t en_ack);
void i2c_send_stop(void);

#endif
