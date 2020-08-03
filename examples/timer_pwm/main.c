
#include "core/pins.h"
#include "timing/timing.h"

int main(void)
{
    // Output compare A = D6
    Pin pwm = PIN_D6;
    gpio_mode_output(pwm);
    timer0_init_as_pwm();
    timer0_set_duty_cycle_a(0.8);
    while (1) {}
}
