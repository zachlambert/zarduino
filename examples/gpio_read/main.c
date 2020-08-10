#include <stdint.h>
#include "core/pins.h"
#include "timing/delay.h"

int main(void)
{
    Pin led = PIN_BUILT_IN_LED;
    Pin button = PIN_D2;
    gpio_mode_output(led);
    gpio_mode_input(button);
    uint8_t value;
    while (1) {
        value = gpio_read(button);
        gpio_write(led, value);
        delay(10);
    }
}
