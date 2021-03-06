#include "zarduino/timing/timing.h"
#include "zarduino/comms/uart.h"

#include <stdint.h>

uint32_t num0 = 0;
uint32_t num1 = 0;
uint32_t num2 = 0;
void callback0(void)
{
    num0++;
}

void callback1(void)
{
    num1++;
}

void callback2(void)
{
    num2++;
    printf("Callbacks %lu %lu %lu\n", num0, num1, num2);
}

int main(void)
{
    uart_init(0);
    printf("Starting\n");

    timer0_init_as_timer_ms(250, callback0);
    timer1_init_as_timer_ms(500, callback1);
    timer2_init_as_timer_ms(1000, callback2);

    while (1) {}
}
