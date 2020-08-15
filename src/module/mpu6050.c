#include "module/mpu6050.h"
#include "comms/i2c.h"
#include "core/regs.h"
#include "timing/timing.h"

const uint8_t SMPRT_DIV_address = 0x19;
const uint8_t CONFIG_address = 0x1A;
const uint8_t PWR_MGMT_1_address = 0x6B;
const uint8_t GYRO_CONFIG_address = 0x1B;
const uint8_t ACCEL_CONFIG_address = 0x1C;

// Accel registers are consecutive, starting from ACCEL_XOUT_H
const uint8_t ACCEL_start_address = 0x3B;
// Gyro registers are consecutive, starting from GYRO_XOUT_H
const uint8_t GYRO_start_address = 0x43;

MPU6050Config mpu6050_create_config(void)
{
    MPU6050Config config = {};
    config.i2c_address = 0x68;
    return config;
}

void mpu6050_init(MPU6050Config *config)
{
    config->i2c_address |= (config->AD0 & 0x01);

    uint8_t PWR_MGMT_1 = 0;
    reg_write_mask(&PWR_MGMT_1, 0, 0b111, config->clock_source);
    i2c_write_register(
        config->i2c_address,
        PWR_MGMT_1_address,
        &PWR_MGMT_1,
        1
    );

    uint8_t SMPRT_DIV = config->sample_rate_divider;
    i2c_write_register(
        config->i2c_address,
        SMPRT_DIV_address,
        &SMPRT_DIV,
        1
    );

    uint8_t CONFIG;
    reg_write_mask(&CONFIG, 0, 0b111, config->low_pass_filter);
    i2c_write_register(
        config->i2c_address,
        CONFIG_address,
        &CONFIG,
        1
    );

    uint8_t GYRO_CONFIG;
    reg_write_mask(&GYRO_CONFIG, 3, 0b11, config->gyro_range);
    i2c_write_register(
        config->i2c_address,
        GYRO_CONFIG_address,
        &GYRO_CONFIG,
        1
    );

    uint8_t ACCEL_CONFIG;
    reg_write_mask(&ACCEL_CONFIG, 3, 0b11, config->accel_range);
    i2c_write_register(
        config->i2c_address,
        ACCEL_CONFIG_address,
        &ACCEL_CONFIG,
        1
    );

    float gyro_range;
    switch (config->gyro_range) {
        case MPU6050_GYRO_RANGE_PM_250_DEGPS:
            gyro_range = 250;
        case MPU6050_GYRO_RANGE_PM_500_DEGPS:
            gyro_range = 500;
        case MPU6050_GYRO_RANGE_PM_1000_DEGPS:
            gyro_range = 1000;
        case MPU6050_GYRO_RANGE_PM_2000_DEGPS:
            gyro_range = 2000;
    }
    // 16-bit signed integer, range = +-32768
    config->gyro_sensitivity = gyro_range/32768.0;

    float accel_range;
    switch (config->gyro_range) {
        case MPU6050_ACCEL_RANGE_PM_2_G:
            accel_range = 2*9.81;
        case MPU6050_ACCEL_RANGE_PM_4_G:
            accel_range = 4*9.81;
        case MPU6050_ACCEL_RANGE_PM_8_G:
            accel_range = 8*9.81;
        case MPU6050_ACCEL_RANGE_PM_16_G:
            accel_range = 16*9.81;
    }
    // 16-bit signed integer, range = +-32768
    config->accel_sensitivity = accel_range/32768.0;
}

void mpu6050_read_data(MPU6050Config *config, MPU6050Data *data)
{
    uint8_t ACCEL[6];
    i2c_read_register(
        config->i2c_address,
        ACCEL_start_address,
        ACCEL,
        6
    );

    uint8_t GYRO[6];
    i2c_read_register(
        config->i2c_address,
        GYRO_start_address,
        GYRO,
        6
    );

    data->accel_x = (float)(ACCEL[0]<<8 | ACCEL[1]) * config->accel_sensitivity;
    data->accel_y = (float)(ACCEL[2]<<8 | ACCEL[3]) * config->accel_sensitivity;
    data->accel_z = (float)(ACCEL[4]<<8 | ACCEL[5]) * config->accel_sensitivity;

    data->gyro_x = (float)(GYRO[0]<<8 | ACCEL[1]) * config->gyro_sensitivity;
    data->gyro_y = (float)(GYRO[2]<<8 | ACCEL[3]) * config->gyro_sensitivity;
    data->gyro_z = (float)(GYRO[4]<<8 | ACCEL[5]) * config->gyro_sensitivity;
}
