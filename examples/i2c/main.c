#include "comms/uart.h"
#include "comms/i2c.h"
#include "timing/delay.h"

int main(void)
{
    I2CConfig i2c_config = i2c_create_config();
    i2c_config.bit_rate_reduction = 0xFF;
    i2c_init_master(&i2c_config);

    uart_init();

    // MPU6050 sensor

    const uint8_t i2c_address = 0x68;

    uint8_t power_management_reg_1 = 0x6B;
    uint8_t power_management_value = 0;
    i2c_write_register(i2c_address, power_management_reg_1, &power_management_value, 1);

    uint8_t accel_start_reg = 0x3B;
    uint8_t accel_data[6];
    float accel[3];

    while (1) {
        i2c_read_register(
            i2c_address, accel_start_reg, accel_data, 6
        );
        for (size_t i = 0; i < 3; i++) {
            accel[i] = (float)(accel_data[2*i]<<8 | accel_data[2*i+1]) / 16384.0;
            // Printf doesn't work with %f at the moment,
            // so scale up and cast to int
            printf("%d ", (int)(accel[i]*1000));
        }
        printf("\n");
        delay(100);
    }
}
