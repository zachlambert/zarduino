
#include <util/delay.h>
#include "core/pins.h"

int main()
{
    IOPin led = ArduinoPin_BUILT_IN_LED;
    gpio_mode_output(led);
    while (1) {
        gpio_write(led, 1);
        _delay_ms(500); 
        gpio_write(led, 0);
        _delay_ms(500);
    }
}
