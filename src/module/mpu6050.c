#include "zarduino/module/mpu6050.h"
#include "zarduino/comms/i2c.h"
#include "zarduino/core/regs.h"
#include "zarduino/comms/uart.h"

#include <math.h>

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
    config.low_pass_filter = MPU6050_LOW_PASS_FILTER_LEVEL6;
    config.clock_source = MPU6050_CLOCK_SOURCE_GYRO_Z;
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
            gyro_range = 250.0f;
            break;
        case MPU6050_GYRO_RANGE_PM_500_DEGPS:
            gyro_range = 500.0f;
            break;
        case MPU6050_GYRO_RANGE_PM_1000_DEGPS:
            gyro_range = 1000.0f;
            break;
        default:
            gyro_range = 2000.0f;
            break;
    }
    // 16-bit signed integer, range = +-32768
    config->gyro_sensitivity = gyro_range/32768.0;

    float accel_range;
    switch (config->gyro_range) {
        case MPU6050_ACCEL_RANGE_PM_2_G:
            accel_range = 2;//*9.81;
            break;
        case MPU6050_ACCEL_RANGE_PM_4_G:
            accel_range = 4;//*9.81;
            break;
        case MPU6050_ACCEL_RANGE_PM_8_G:
            accel_range = 8;//*9.81;
            break;
        default:
            accel_range = 16;//*9.81;
            break;
    }
    // 16-bit signed integer, range = +-32768
    config->accel_sensitivity = accel_range/32768.0;

    mpu6050_calibrate(config);
}

void mpu6050_read_accel(MPU6050Config *config, float accel[3])
{
    uint8_t ACCEL[6];
    i2c_read_register(config->i2c_address, ACCEL_start_address, ACCEL, 6);

    for (size_t i = 0; i < 3; i++)
        accel[i] = (float)(ACCEL[2*i]<<8 | ACCEL[2*i+1]) * config->accel_sensitivity;
}

void mpu6050_read_gyro(MPU6050Config *config, float gyro[3])
{
    uint8_t GYRO[6];
    i2c_read_register(config->i2c_address, GYRO_start_address, GYRO, 6);

    for (size_t i = 0; i < 3; i++)
        gyro[i] = ((float)(GYRO[2*i]<<8 | GYRO[2*i+1])) * config->gyro_sensitivity;
}

void mpu6050_read_data(MPU6050Config *config, MPU6050Data *data)
{
    mpu6050_read_accel(config, data->accel);
    for (size_t i = 0; i < 2; i++)
        data->accel[i] -= config->accel_zero_readings[i];

    mpu6050_read_gyro(config, data->gyro);
    for (size_t i = 0; i < 3; i++)
        data->gyro[i] -= config->gyro_zero_readings[i];
}

void mpu6050_calibrate(MPU6050Config *config)
{
    mpu6050_read_accel(config, config->accel_zero_readings);
    config->accel_zero_readings[2] -= 9.81; // Should be 9.81
    mpu6050_read_gyro(config, config->gyro_zero_readings);
}

void mpu6050_calculate_euler(MPU6050Data* data)
{
    float axy = hypot(data->accel[0], data->accel[1]);
    data->tilt = atan2(axy, data->accel[2]);
    data->roll = atan2(data->accel[1], data->accel[0]);
}
