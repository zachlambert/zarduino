#include <string.h>
#include "comms/uart.h"
#include "timing/delay.h"
#include "core/pins.h"
#include <stdio.h>

int main(void)
{
    Pin led = PIN_ARDUINO_LED;
    uart_init(); 
    char value;
    gpio_mode_output(led);
    while (1) {
        scanf("%c", &value);
        if (value & 0x01) {
            gpio_write(led, 1);
        } else {
            gpio_write(led, 0);
        }
    }
}
