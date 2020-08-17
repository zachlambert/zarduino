
#include "zarduino/core/pins.h"
#include "zarduino/timing/timing.h"

int main(void)
{
    // Output compare A = D6
    Pin pwm0a = PIN_TIMER0_A;
    Pin pwm0b = PIN_TIMER0_B;
    Pin pwm1a = PIN_TIMER1_A;
    Pin pwm1b = PIN_TIMER1_B;
    Pin pwm2a = PIN_TIMER2_A;
    Pin pwm2b = PIN_TIMER2_B;
    gpio_mode_output(pwm0a);
    gpio_mode_output(pwm0b);
    gpio_mode_output(pwm1a);
    gpio_mode_output(pwm1b);
    gpio_mode_output(pwm2a);
    gpio_mode_output(pwm2b);

    timer0_init_as_pwm();
    timer0_set_duty_cycle_a(0.8); //D6
    timer0_set_duty_cycle_b(0.2); //D5
    
    timer1_init_as_pwm();
    timer1_set_duty_cycle_a(0.7); //D9
    timer1_set_duty_cycle_b(0.3); //D10

    timer2_init_as_pwm();
    timer2_set_duty_cycle_a(0.6); //D11
    timer2_set_duty_cycle_b(0.4); //D3

    while (1) {}
}
