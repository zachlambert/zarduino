#include "core/pins.h"
#include "timing/delay.h"
#include "comms/uart.h"
#include "core/adc.h"
#include <avr/io.h>

int main(void)
{
    IOPin led = PIN_BUILT_IN_LED;
    uart_init();

    ADCConfig config = adc_create_config();
    adc_initialise(&config);

    while (1) {
        printf("%u\n", adc_read_wait());
    }
}
