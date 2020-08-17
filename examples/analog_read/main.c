#include "zarduino/comms/uart.h"
#include "zarduino/core/adc.h"
#include <avr/io.h>

int main(void)
{
    uart_init();
    ADCConfig config = adc_create_config();
    adc_initialise(&config);

    while (1) {
        printf("%u\n", adc_read_wait());
    }
}
