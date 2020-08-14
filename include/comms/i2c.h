#ifndef ZARDUINO_COMMS_I2C_H
#define ZARDUINO_COMMS_I2C_H

#include <stdint.h>

typedef struct {

} I2CConfig;

I2CConfig i2c_create_config(void);
void i2c_init_master(I2CConfig *config);

uint8_t i2c_read(uint8_t i2c_address);
void i2c_write(uint8_t i2c_address, uint8_t value);

#endif
