#include "module/oled.h"
#include "comms/i2c.h"

int main(void)
{
    I2CConfig i2c_config = i2c_create_config();
    i2c_init_master(&i2c_config);

    OLEDConfig oled_config = oled_create_config();
    oled_init(&oled_config);

    OLEDData oled_data = {};
    oled_putc(&oled_data, '!');
    oled_update(&oled_config, &oled_data);
}
