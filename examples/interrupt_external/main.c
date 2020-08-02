#include "core/pins.h"
#include "core/interrupt.h"
#include "comms/uart.h"

int num;
void callback(void)
{
    printf("External callback %d\n", num++);
}

int main(void)
{
    uart_init();
    printf("Starting\n");
    interrupt_external_add_callback(
        INTERRUPT_EXTERNAL_0,
        INTERRUPT_TYPE_FALLING,
        callback
    );
    while (1) {}
}
