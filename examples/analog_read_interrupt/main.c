#include "zarduino/comms/uart.h"
#include "zarduino/core/adc.h"
#include <avr/io.h>

void adc_callback(uint16_t value)
{
    printf("%d\n", value);
}

int main(void)
{
    uart_init();

    ADCConfig config = adc_create_config();
    adc_initialise(&config);

    adc_read_callback_repeat(adc_callback);
    // Put the code in an infinite loop here, so the
    // program doesn't terminate
    while (1) {}
}
