#include "zarduino/comms/uart.h"
#include "zarduino/core/adc.h"
#include <avr/io.h>

int main(void)
{
    uart_init(0);
    ADCConfig config = adc_create_config();
    adc_initialise(&config);
    Pin adc_pin = PIN_ARDUINO_A0;

    while (1) {
        printf("%u\n", adc_read_wait(adc_pin));
    }
}
