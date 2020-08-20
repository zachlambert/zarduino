#include "zarduino/module/oled.h"
#include "zarduino/comms/i2c.h"
#include "zarduino/timing/delay.h"
#include <stdio.h>

int main(void)
{
    I2CConfig i2c_config = i2c_create_config();
    i2c_init_master(&i2c_config);

    OLEDConfig oled_config = oled_create_config();
    oled_init(&oled_config);

    for (unsigned char c = 32; c <= 126; c++)
        oled_putc(&oled_config, c);
    oled_flush(&oled_config);
    oled_clear(&oled_config);

    delay(1000);

    const size_t STR_BUF_LEN = 100;
    char line1[STR_BUF_LEN];
    char line2[STR_BUF_LEN];
    int counter1 = 0;
    int counter2 = 0;
    while (1) {
        snprintf(line1, STR_BUF_LEN, "LINE 1: %d\n", counter1);
        snprintf(line2, STR_BUF_LEN, "LINE 2: %d\n", counter2);
        oled_print_string(&oled_config, line1);
        oled_print_string(&oled_config, line2);
        oled_clear(&oled_config);

        counter1++;
        if (counter1 >= 10) {
            counter1 = 0;
            counter2++;
        }
        delay(100);
    }
}
