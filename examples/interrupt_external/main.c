#include "core/pins.h"
#include "core/interrupt.h"
#include "comms/uart.h"

void callback(void)
{
    printf("External callback\n");
}

int main(void)
{
    uart_init();
    interrupt_external_add_callback(
        INTERRUPT_EXTERNAL_0,
        INTERRUPT_TYPE_FALLING,
        callback
    );
    while (1) {}
}
