
#include "comms/uart.h"
#include "module/mpu6050.h"
#include "timing/delay.h"

int main(void)
{
    uart_init();

    MPU6050Config config = mpu6050_create_config();
    mpu6050_init(&config);

    MPU6050Data data = {};

    while (1) {
        mpu6050_read_data(&config, &data);
        printf(
            "Acc: %d %d %d ",
            (int)data.accel_x,
            (int)data.accel_y,
            (int)data.accel_z
        );
        printf(
            "Gyro: %d %d %d\n",
            (int)data.gyro_x,
            (int)data.gyro_y,
            (int)data.gyro_z
        );
        delay(50);
    }
}
