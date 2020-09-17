#include "zarduino/core/pins.h"
#include "zarduino/core/interrupt.h"
#include "zarduino/comms/uart.h"

int num;
void callback(void)
{
    printf("External callback %d\n", num++);
}

int main(void)
{
    uart_init(0);
    printf("Starting\n");
    interrupt_external_add_callback(
        INTERRUPT_EXTERNAL_0,
        INTERRUPT_TYPE_FALLING,
        callback
    );
    while (1) {}
}
