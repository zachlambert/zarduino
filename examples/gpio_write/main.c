
#include "zarduino/core/pins.h"
#include "zarduino/timing/delay.h"
#include <avr/io.h>

int main(void)
{
    Pin led = PIN_PD4;
    gpio_mode_output(led);
    while (1) {
        gpio_write(led, 1);
        delay(500);
        gpio_write(led, 0);
        delay(500);
    }
}
