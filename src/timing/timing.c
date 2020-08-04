#include "timing/timing.h"
#include "core/regs.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// TIMER/COUNTERs
//
// There are 3 timer/counters, which have a
// counter.
// The counter has some clock source, and counts up
// to a number of clock cycles, given by: TCNTn
// TCNTn has values from BOT (0) -> TOP
// TOP is a configurable period length, in number of cycles
// and has a max value of MAX
//
// Timer 0 has a 8-bit timer/counter
// Timer 1 has a 16-bit timer/counter
//
//
// The timers can be configured with a clock source, which
// is the cpu clock with some division, or an external
// clock source.
//
// An interrupt can be triggered on the counter oveflow,
// when it reaches TOP
//
// Output compare can be enabled on the timers, which is
// a configurable level within the clock period.
// - An output compare signal is generated, such as
//   setting it to 1 when timer < output comare, to
//   generate a PWM signal
// - An interrupt can be triggered when the output compare
//   is reached
//
// There are 2 output compares per timer/counter,
// so 6 possible hardware PWM outputs, where each
// pair has to have the same period
//
// Timer 2 has an input capture unit, which can be used
// for measuring duty cycle or period
//
// Timer 3 can use an external clock, which allows it to
// operate asynchronously from the cpu clock
//
// How to configure these timer/counters ?
// 1. Configure each separately, and give a single
//    function.
// 2. Create functions for configuring with specific
//    functions
//
// Common functions:
// - Configure PWM, write duty cycle for either channel
// - Configure timer with interrupt in timer overflow
//
// Do later:
// - Configure specific period and duty cycle
// - For timer/counter 0, use the input capture


// ===== Enums =====

typedef enum {
    OUTPUT_COMPARE_MODE_DISABLED,
    OUTPUT_COMPARE_MODE_TOGGLE,
    OUTPUT_COMPARE_MODE_CLEAR,
    OUTPUT_COMPARE_MODE_SET,
} OutputCompareMode;

typedef enum {
    TIMER0_WAVEFORM_NORMAL = 0,
    TIMER0_WAVEFORM_PWM_PHASE_CORRECT = 1,
    TIMER0_WAVEFORM_CTC = 2,
    TIMER0_WAVEFORM_PWM_FAST = 3,
    TIMER0_WAVEFORM_PWM_PHASE_CORRECT_OCRA = 5,
    TIMER0_WAVEFORM_PWM_FAST_OC = 7,
} Timer0Waveform;

typedef enum {
    TIMER1_WAVEFORM_NORMAL = 0,
    TIMER1_WAVEFORM_PWM_PHASE_CORRECT_8 = 1,
    TIMER1_WAVEFORM_PWM_PHASE_CORRECT_9 = 2,
    TIMER1_WAVEFORM_PWM_PHASE_CORRECT_10 = 3,
    TIMER1_WAVEFORM_CTC_OCR1A = 4,
    TIMER1_WAVEFORM_PWM_FAST_8 = 5,
    TIMER1_WAVEFORM_PWM_FAST_9 = 6,
    TIMER1_WAVEFORM_PWM_FAST_10 = 7,
    TIMER1_WAVEFORM_PWM_PHASE_FREQ_CORRECT_ICR1 = 8,
    TIMER1_WAVEFORM_PWM_PHASE_FREQ_CORRECT_OCR1A = 9,
    TIMER1_WAVEFORM_PWM_PHASE_CORRECT_ICR1 = 10,
    TIMER1_WAVEFORM_PWM_PHASE_CORRECT_OCR1A = 11,
    TIMER1_WAVEFORM_CTC_ICR1 = 12,
    TIMER1_WAVEFORM_PWM_FAST_ICR1 = 14,
    TIMER1_WAVEFORM_PWM_FAST_OCR1A = 15
} Timer1Waveform;

typedef enum {
    TIMER0_CLOCK_NONE,
    TIMER0_CLOCK_CLK,
    TIMER0_CLOCK_CLK_DIV_8,
    TIMER0_CLOCK_CLK_DIV_64,
    TIMER0_CLOCK_CLK_DIV_256,
    TIMER0_CLOCK_CLK_DIV_1024,
    TIMER0_CLOCK_EXTERNAL_FALLING,
    TIMER0_CLOCK_EXTERNAL_RISING
} Timer0Clock;

typedef enum {
    TIMER1_CLOCK_NONE,
    TIMER1_CLOCK_CLK,
    TIMER1_CLOCK_CLK_DIV_8,
    TIMER1_CLOCK_CLK_DIV_64,
    TIMER1_CLOCK_CLK_DIV_256,
    TIMER1_CLOCK_CLK_DIV_1024,
    TIMER1_CLOCK_EXTERNAL_FALLING,
    TIMER1_CLOCK_EXTERNAL_RISING
} Timer1Clock;

typedef enum {
    TIMER2_CLOCK_NONE,
    TIMER2_CLOCK_CLK,
    TIMER2_CLOCK_CLK_DIV_8,
    TIMER2_CLOCK_CLK_DIV_32,
    TIMER2_CLOCK_CLK_DIV_64,
    TIMER2_CLOCK_CLK_DIV_128,
    TIMER2_CLOCK_CLK_DIV_256,
    TIMER2_CLOCK_CLK_DIV_1024
} Timer2Clock;


// ===== TIMER0 PWM =====

void timer0_init_as_pwm(void)
{
    // Clear OC0A and 0C0B on compare match
    reg_write_mask( // Control register A, channel A
        &TCCR0A, 0b11000000,
        OUTPUT_COMPARE_MODE_CLEAR << 6
    );
    reg_write_mask( // Control register A, channel B
        &TCCR0A, 0b00110000,
        OUTPUT_COMPARE_MODE_CLEAR << 4
    );

    // Use waveform generation PWM fast, which with the
    // output compare mode:
    // - Sets output compare on BOTTOM
    // - Clears output compare on compare match
    uint8_t waveform = TIMER0_WAVEFORM_PWM_FAST;
    reg_write_bit(&TCCR0B, WGM02, (waveform & 0b100) >> 2);
    reg_write_bit(&TCCR0A, WGM01, (waveform & 0b010) >> 1);
    reg_write_bit(&TCCR0A, WGM00, (waveform & 0b001) >> 0);

    // Set clock source to CLK
    uint8_t clock = TIMER0_CLOCK_CLK;
    reg_write_mask(&TCCR0B, 0b00000111, clock);
}

void timer0_set_duty_cycle_a(float duty_cycle)
{
    uint8_t compare_value = (uint8_t)(duty_cycle * 256);
    OCR0A = compare_value;
}

void timer0_set_duty_cycle_b(float duty_cycle)
{
    uint8_t compare_value = (uint8_t)(duty_cycle * 256);
    OCR0B = compare_value;
}


// ===== TIMER0 Timer =====

void (*callback_timer0)(void);
uint64_t counter_timer0;
uint64_t counter_max_timer0;
uint8_t timer0_exact = 0;
ISR(TIMER0_OVF_vect)
{
    if (!timer0_exact && counter_timer0 == counter_max_timer0) {
        reg_write_bit(&TIMSK0, OCIE0A, 1);
    }
    counter_timer0++;
}
ISR(TIMER0_COMPA_vect)
{
    callback_timer0();
    TCNT0 = 0;
    if (counter_max_timer0) {
        counter_timer0 = 0;
        reg_write_bit(&TIMSK0, OCIE0A, 0);
    }
}

void timer0_init_as_timer_ms(float ms, void (*callback)(void))
{
    // Don't use output compare on either channel
    reg_write_mask(&TCCR0A, 0b11000000, 0);
    reg_write_mask(&TCCR0A, 0b00110000, 0);

    // Normal timer/counter waveform
    reg_write_bit(&TCCR0B, WGM02, 0);
    reg_write_bit(&TCCR0A, WGM01, 0);
    reg_write_bit(&TCCR0A, WGM00, 0);

    uint8_t clock = TIMER0_CLOCK_CLK_DIV_1024;
    reg_write_mask(&TCCR0B, 0b00000111, clock);

    uint64_t clock_cycles = (ms * F_CPU)/(1000.0f*1024);
    counter_timer0 = 0;
    counter_max_timer0 = clock_cycles / 256;
    OCR0A = clock_cycles % 256;

    // Enable interrupts
    sei();
    callback_timer0 = callback;
    if (counter_max_timer0) {
        reg_write_bit(&TIMSK0, TOIE0, 1);
    } else {
        reg_write_bit(&TIMSK0, OCIE0A, 1);
    }
}

void timer0_init_as_timer_accurate(void)
{
    reg_write_mask(&TCCR0A, 0b11000000, 0);
    reg_write_mask(&TCCR0A, 0b00110000, 0);

    reg_write_bit(&TCCR0B, WGM02, 0);
    reg_write_bit(&TCCR0A, WGM01, 0);
    reg_write_bit(&TCCR0A, WGM00, 0);

    uint8_t clock = TIMER0_CLOCK_CLK;
    reg_write_mask(&TCCR0B, 0b00000111, clock);

    // Enable interrupts
    sei();
    timer0_exact = 1;
    reg_write_bit(&TIMSK0, TOIE0, 1);
}

uint64_t timer0_get_accurate_millis(void)
{
    // counter_timer0 = units of (256/F_CPU) = 16us
    // Factor = 1ms / 16us = 62.5
    return (uint64_t)((float)counter_timer0 / 62.5f);
}

// ===== TIMER1 PWM =====

void timer1_init_as_pwm(void)
{
    reg_write_mask(
        &TCCR1A, 0b11000000,
        OUTPUT_COMPARE_MODE_CLEAR << 6
    );
    reg_write_mask(
        &TCCR1A, 0b00110000,
        OUTPUT_COMPARE_MODE_CLEAR << 4
    );
    uint8_t waveform = TIMER1_WAVEFORM_PWM_FAST_10;
    reg_write_bit(&TCCR1B, WGM13, (waveform & 0b1000) >> 3);
    reg_write_bit(&TCCR1B, WGM12, (waveform & 0b0100) >> 2);
    reg_write_bit(&TCCR1A, WGM11, (waveform & 0b0010) >> 1);
    reg_write_bit(&TCCR1A, WGM10, (waveform & 0b0001) >> 0);

    uint8_t clock = TIMER1_CLOCK_CLK;
    reg_write_mask(&TCCR1B, 0b00000111, clock);
}

void timer1_set_duty_cycle_a(float duty_cycle)
{
    uint16_t compare_value = (uint16_t)(duty_cycle * 1024);
    OCR1A = compare_value;
}

void timer1_set_duty_cycle_b(float duty_cycle)
{
    uint16_t compare_value = (uint16_t)(duty_cycle * 1024);
    OCR1B = compare_value;
}


// ===== TIMER1 Timer =====

// Since Timer1 is 16-bit, so need to use the compare
// match interrupt.
// However, such that the max time isn't limited, the timer
// interrupts on compare match to achieve a certain period

void (*callback_timer1)(void);
uint64_t counter_timer1;
uint64_t counter_max_timer1;
ISR(TIMER1_OVF_vect)
{
    if (counter_timer1 == counter_max_timer1) {
        reg_write_bit(&TIMSK1, OCIE1A, 1);
    }
    counter_timer1++;
}
ISR(TIMER1_COMPA_vect)
{
    callback_timer1();
    TCNT1 = 0;
    if (counter_max_timer1) {
        counter_timer1 = 0;
        reg_write_bit(&TIMSK1, OCIE1A, 0);
    }
}

void timer1_init_as_timer_ms(float ms, void (*callback)(void))
{
    // Don't need either output compare signal, but
    // will use the interrupt on compare match A
    reg_write_mask(&TCCR1A, 0b11000000, 0);
    reg_write_mask(&TCCR1A, 0b00110000, 0);

    // Can still use the normal waveform
    uint8_t waveform = TIMER1_WAVEFORM_NORMAL;
    reg_write_bit(&TCCR1B, WGM13, (waveform & 0b1000) >> 3);
    reg_write_bit(&TCCR1B, WGM12, (waveform & 0b0100) >> 2);
    reg_write_bit(&TCCR1A, WGM11, (waveform & 0b0010) >> 1);
    reg_write_bit(&TCCR1A, WGM10, (waveform & 0b0001) >> 0);

    uint8_t clock = TIMER1_CLOCK_CLK_DIV_1024;
    reg_write_mask(&TCCR1B, 0b00000111, clock);

    uint64_t clock_cycles = (ms * F_CPU)/(1000.0f*1024);
    counter_timer1 = 0;
    counter_max_timer1 = clock_cycles / 65536;
    OCR1A = clock_cycles % 65536;
    // Enable interrupts
    sei();
    callback_timer1 = callback;
    if (counter_max_timer1) {
        reg_write_bit(&TIMSK1, TOIE1, 1);
    } else {
        reg_write_bit(&TIMSK1, OCIE1A, 1);
    }
}


// ===== TIMER2 PWM =====

void timer2_init_as_pwm(void)
{
    reg_write_mask(
        &TCCR2A, 0b11000000,
        OUTPUT_COMPARE_MODE_CLEAR << 6
    );
    reg_write_mask(
        &TCCR2A, 0b00110000,
        OUTPUT_COMPARE_MODE_CLEAR << 4
    );
    uint8_t waveform = TIMER0_WAVEFORM_PWM_FAST;
    reg_write_bit(&TCCR2B, WGM22, (waveform & 0b100) >> 2);
    reg_write_bit(&TCCR2A, WGM21, (waveform & 0b010) >> 1);
    reg_write_bit(&TCCR2A, WGM20, (waveform & 0b001) >> 0);

    uint8_t clock = TIMER2_CLOCK_CLK;
    reg_write_mask(&TCCR2B, 0b00000111, clock);
}

void timer2_set_duty_cycle_a(float duty_cycle)
{
    uint8_t compare_value = (uint8_t)(duty_cycle * 256);
    OCR2A = compare_value;
}

void timer2_set_duty_cycle_b(float duty_cycle)
{
    uint8_t compare_value = (uint8_t)(duty_cycle * 256);
    OCR2B = compare_value;
}


// ===== TIMER2 Timer =====

void (*callback_timer2)(void);
uint64_t counter_timer2;
uint64_t counter_max_timer2;
ISR(TIMER2_OVF_vect)
{
    if (counter_timer2 == counter_max_timer2) {
        reg_write_bit(&TIMSK2, OCIE2A, 1);
    }
    counter_timer2++;
}
ISR(TIMER2_COMPA_vect)
{
    callback_timer2();
    TCNT2 = 0;
    if (counter_max_timer2) {
        counter_timer2 = 0;
        reg_write_bit(&TIMSK2, OCIE2A, 0);
    }
}

void timer2_init_as_timer_ms(float ms, void (*callback)(void))
{
    // Don't use output compare on either channel
    reg_write_mask(&TCCR2A, 0b11000000, 0);
    reg_write_mask(&TCCR2A, 0b00110000, 0);

    // Normal timer/counter waveform
    reg_write_bit(&TCCR2B, WGM22, 0);
    reg_write_bit(&TCCR2A, WGM21, 0);
    reg_write_bit(&TCCR2A, WGM20, 0);

    uint8_t clock = TIMER2_CLOCK_CLK_DIV_1024;
    reg_write_mask(&TCCR2B, 0b00000111, clock);

    uint64_t clock_cycles = (ms * F_CPU)/(1000.0f*1024);
    counter_timer2 = 0;
    counter_max_timer2 = clock_cycles / 256;
    OCR2A = clock_cycles % 256;

    // Enable interrupts
    sei();
    callback_timer2 = callback;
    if (counter_max_timer2) {
        reg_write_bit(&TIMSK2, TOIE2, 1);
    } else {
        reg_write_bit(&TIMSK2, OCIE2A, 1);
    }

}
