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
        printf("Millis: %lu\n", timer0_get_accurate_millis());
        delay(500);
    }
}
