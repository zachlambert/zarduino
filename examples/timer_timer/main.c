#include "timing/timing.h"
#include "comms/uart.h"

int num = 0;
void callback(void)
{
    printf("Callback %d\n", num++);    
}

int main(void)
{
    uart_init();
    printf("Starting\n");
    timer0_init_as_timer_ms(250, callback);
    while (1) {}
}
