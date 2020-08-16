#include "module/oled.h"
#include "comms/i2c.h"
#include <string.h>
#include "timing/delay.h"

int main(void)
{
    I2CConfig i2c_config = i2c_create_config();
    i2c_init_master(&i2c_config);

    OLEDConfig oled_config = oled_create_config();
    oled_init(&oled_config);

    OLEDData oled_data = oled_create_data(&oled_config);

    for (unsigned char c = 32; c <= 126; c++)
        oled_putc(&oled_config, &oled_data, c);

    while (1) {
        oled_update(&oled_config, &oled_data);
        delay(50);
    }
}
