
#include "core/pins.h"
#include "core/interrupt.h"
#include "comms/uart.h"

int num;
void callback(void)
{
    printf("Pin callback %d\n", num++);
}

int main(void)
{
    uart_init();
    Pin interrupt_pin = PIN_D10;
    interrupt_pin_add_callback(
        interrupt_pin,
        callback
    );
    while (1) {}
}
