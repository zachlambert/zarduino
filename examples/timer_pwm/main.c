
#include "core/pins.h"
#include "timing/timing.h"

int main(void)
{
    // Output compare A = D6
    Pin pwm0a = PIN_TIMER0_A_D6;
    Pin pwm0b = PIN_TIMER0_B_D5;
    Pin pwm1a = PIN_TIMER1_A_D9;
    Pin pwm1b = PIN_TIMER1_B_D10;
    Pin pwm2a = PIN_TIMER2_A_D11;
    Pin pwm2b = PIN_TIMER2_B_D3;
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
