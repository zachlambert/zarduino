#ifndef ZARDUINO_MODULE_MPU6050
#define ZARDUINO_MODULE_MPU6050

#include <stdint.h>

typedef enum {
    MPU6050_CLOCK_SOURCE_INTERNAL,
    MPU6050_CLOCK_SOURCE_GYRO_X,
    MPU6050_CLOCK_SOURCE_GYRO_Y,
    MPU6050_CLOCK_SOURCE_GYRO_Z,
    MPU6050_CLOCK_SOURCE_GYRO_EXTERNAL_33KHZ,
    MPU6050_CLOCK_SOURCE_GYRO_EXTERNAL_19MHZ,
    // reserved
    MPU6050_CLOCK_SOURCE_STOPPED = 0b111
} MPU6050ClockSource;

// The low pass filter affects the sample rate of
// the gyroscope.
// Accelerometer always has sample rate = 1kHz
// Gyroscope has sample rate 8KHz if low pass filter is
// disabled, and 1KHz if the enabled
typedef enum {
    MPU6050_LOW_PASS_FILTER_DISABLED, // ~260 Hz bandwidth
    MPU6050_LOW_PASS_FILTER_LEVEL1, // ~184Hz bandwidth
    MPU6050_LOW_PASS_FILTER_LEVEL2,
    MPU6050_LOW_PASS_FILTER_LEVEL3,
    MPU6050_LOW_PASS_FILTER_LEVEL4,
    MPU6050_LOW_PASS_FILTER_LEVEL5,
    MPU6050_LOW_PASS_FILTER_LEVEL6 // ~5Hz bandwidth
    // reserved
} MPU6050LowPassFilter;

// plus or minux X degrees per second
typedef enum {
    MPU6050_GYRO_RANGE_PM_250_DEGPS,
    MPU6050_GYRO_RANGE_PM_500_DEGPS,
    MPU6050_GYRO_RANGE_PM_1000_DEGPS,
    MPU6050_GYRO_RANGE_PM_2000_DEGPS
} MPU6050GyroRange;

// plus or minux X g
typedef enum {
    MPU6050_ACCEL_RANGE_PM_2_G,
    MPU6050_ACCEL_RANGE_PM_4_G,
    MPU6050_ACCEL_RANGE_PM_8_G,
    MPU6050_ACCEL_RANGE_PM_16_G
} MPU6050AccelRange;

typedef struct {
    MPU6050ClockSource clock_source;
    MPU6050LowPassFilter low_pass_filter;
    uint8_t sample_rate_divider;
    MPU6050GyroRange gyro_range;
    MPU6050AccelRange accel_range;
    uint8_t AD0;
    uint8_t i2c_address;
    float accel_sensitivity;
    float gyro_sensitivity;
} MPU6050Config;

typedef struct {
    float gyro_x;
    float gyro_y;
    float gyro_z;
    float accel_x;
    float accel_y;
    float accel_z;
} MPU6050Data;

MPU6050Config mpu6050_create_config(void);
void mpu6050_init(MPU6050Config *config);
void mpu6050_read_data(MPU6050Config *config, MPU6050Data *data);

#endif
