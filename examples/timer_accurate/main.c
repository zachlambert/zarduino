#include "timing/timing.h"
#include "timing/delay.h"
#include "comms/uart.h"

#include <stdint.h>

int main(void)
{
    uart_init();
    printf("Starting\n");

    timer0_init_as_timer_accurate();
    while (1) {
        printf("Ticks: %lu\n", (uint32_t)timer0_accurate_get_ticks());
        delay(500);
    }
}
