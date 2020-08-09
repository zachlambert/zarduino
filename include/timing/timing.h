#ifndef ZARDUINO_TIMING_TIMING
#define ZARDUINO_TIMING_TIMING

#include <stdint.h>

void timer0_init_as_pwm(void);
void timer0_set_duty_cycle_a(float duty_cycle);
void timer0_set_duty_cycle_b(float duty_cycle);
void timer0_init_as_timer_ms(float ms, void (*callback)(void));
void timer0_init_as_timer_accurate(void);
void timer0_accurate_reset(void);
uint64_t timer0_accurate_get_ticks(void);

void timer1_init_as_pwm(void);
void timer1_set_duty_cycle_a(float duty_cycle);
void timer1_set_duty_cycle_b(float duty_cycle);
void timer1_init_as_timer_ms(float ms, void (*callback)(void));
// Not sure if the pulse timing stuff works
void timer1_init_as_pulse_capture_us(void);
uint16_t timer1_get_pulse_length_us(void);

void timer2_init_as_pwm(void);
void timer2_set_duty_cycle_a(float duty_cycle);
void timer2_set_duty_cycle_b(float duty_cycle);
void timer2_init_as_timer_ms(float ms, void (*callback)(void));

#endif
