
#include "comms/i2c.h"
#include "core/regs.h"
#include <avr/interrupt.h>

I2CConfig i2c_create_config(void)
{
    I2CConfig config = {};
    // Set default parameters
    return config;
}

void i2c_init_master(I2CConfig *config)
{

}

uint8_t i2c_read(uint8_t i2c_address)
{
    return 0;
}

void i2c_write(uint8_t i2c_address, uint8_t value)
{

}

