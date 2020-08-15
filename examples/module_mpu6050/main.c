
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
        mpu6050_calculate_euler(&data);
        printf("Acc: ");
        for(size_t i = 0; i < 3; i++)
            printf("%d ", (int)data.accel[i]);
        printf(" | Gyro: ");
        for(size_t i = 0; i < 3; i++)
            printf("%d ", (int)data.gyro[i]);
        printf(" | Tilt: %d | Roll: %d", (int)(data.tilt*57.3), (int)(data.roll*57.3));
        printf("\n");
        delay(50);
    }
}
