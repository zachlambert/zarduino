#include "timing/timing.h"
#include "timing/delay.h"
#include "core/pins.h"
#include "core/interrupt.h"
#include "comms/uart.h"

#include <stdint.h>

Pin trigger, echo;
uint64_t value = 0;
void echo_callback(void)
{
    if (!gpio_read(echo)) {
        value = timer0_accurate_get_ticks();
    } else {
        timer0_accurate_reset();
    }
}

int main(void)
{
    uart_init();
    printf("Starting\n");

    trigger = PIN_D3;
    echo = PIN_D2;
    gpio_mode_output(trigger);

    // Using HC-SR04 ultrasound distance sensor
    // Send 10us signal
    // Time the echo signal, where high time = time of flight
    // for sound, so distance = time * 330m/s /2
    // Recommends minimum 60ms between measurements
    timer0_init_as_timer_accurate();
    interrupt_external_add_callback(
        INTERRUPT_EXTERNAL_0,
        INTERRUPT_TYPE_ANY,
        echo_callback
    );

    while (1) {
        // gpio_write(trigger, 1);
        gpio_write(trigger, 1);
        delay_us(10);
        gpio_write(trigger, 0);
        delay(50);
        printf("%lu\n", (uint32_t)((double)value*0.5 * 62.5e-6 * 330));
        delay(10);
    }
}
