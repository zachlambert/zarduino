
#include "zarduino/core/pins.h"
#include "zarduino/core/interrupt.h"
#include "zarduino/comms/uart.h"

int num;
void callback(void)
{
    printf("Pin callback %d\n", num++);
}

int main(void)
{
    uart_init(0);
    Pin interrupt_pin = PIN_ARDUINO_D10;
    interrupt_pin_add_callback(
        interrupt_pin,
        callback
    );
    while (1) {}
}
