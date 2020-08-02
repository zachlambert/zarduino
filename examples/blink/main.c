
#include "core/pins.h"
#include "timing/delay.h"

int main(void)
{
    Pin led = PIN_BUILT_IN_LED;
    gpio_mode_output(led);
    while (1) {
        gpio_write(led, 1);
        delay(500);
        gpio_write(led, 0);
        delay(500);
    }
}
