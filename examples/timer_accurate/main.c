#include "zarduino/timing/timing.h"
#include "zarduino/timing/delay.h"
#include "zarduino/comms/uart.h"

#include <stdint.h>

int main(void)
{
    uart_init();
    printf("Starting\n");

    timer0_init_as_timer_accurate();
    while (1) {
        printf("Ticks: %lu\n", (uint32_t)((float)timer0_accurate_get_ticks()*64e-9));
        delay(100);
    }
}
