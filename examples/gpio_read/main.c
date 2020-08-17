#include <stdint.h>
#include "zarduino/core/pins.h"
#include "zarduino/timing/delay.h"

int main(void)
{
    Pin led = PIN_ARDUINO_LED;
    Pin button = PIN_ARDUINO_D2;
    gpio_mode_output(led);
    gpio_mode_input(button);
    uint8_t value;
    while (1) {
        value = gpio_read(button);
        gpio_write(led, value);
        delay(10);
    }
}
