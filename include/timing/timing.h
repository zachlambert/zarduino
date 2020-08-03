#ifndef ZARDUINO_TIMING_TIMING
#define ZARDUINO_TIMING_TIMING

#include <stdint.h>

void timer0_init_as_pwm(void);
void timer0_set_duty_cycle_a(float duty_cycle);
void timer0_set_duty_cycle_b(float duty_cycle);
void timer0_init_as_timer_ms(uint32_t ms, void (*callback)(void));

void timer1_init_as_pwm(void);
void timer1_set_duty_cycle_a(float duty_cycle);
void timer1_set_duty_cycle_b(float duty_cycle);
void timer1_init_as_timer_ms(uint32_t ms, void (*callback)(void));

void timer2_init_as_pwm(void);
void timer2_set_duty_cycle_a(float duty_cycle);
void timer2_set_duty_cycle_b(float duty_cycle);
void timer2_init_as_timer_ms(uint32_t ms, void (*callback)(void));

#endif
