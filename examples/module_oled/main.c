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

    oled_clear();
    oled_putc(&oled_config, '!');

    while (1) {
        oled_update(&oled_config);
        delay(50);
    }
}
