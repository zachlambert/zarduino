#include "core/pins.h"
#include "core/adc.h"
#include "timing/delay.h"

int main(void)
{
    IOPin led = PIN_BUILT_IN_LED;
    IOPin analog_input = PIN_A0;
    gpio_mode_output(led);
    adc_initialise(0);
    uint16_t value;
    while (1) {
        value = adc_read_polling(analog_input);
        if (value > 500) {
            gpio_write(led, 1);
        } else {
            gpio_write(led, 0);
        }
        delay(100);
    }
}
