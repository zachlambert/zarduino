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
    TIMER1_WAVEFORM_NORMAL,
    TIMER1_WAVEFORM_PWM_PHASE_CORRECT_8,
    TIMER1_WAVEFORM_PWM_PHASE_CORRECT_9,
    TIMER1_WAVEFORM_PWM_PHASE_CORRECT_10,
    TIMER1_WAVEFORM_CTC_OCR1A,
    TIMER1_WAVEFORM_PWM_FAST_8,
    TIMER1_WAVEFORM_PWM_FAST_9,
    TIMER1_WAVEFORM_PWM_FAST_10,
    TIMER1_WAVEFORM_PWM_PHASE_FREQ_CORRECT_ICR1,
    TIMER1_WAVEFORM_PWM_PHASE_FREQ_CORRECT_OCR1A,
    TIMER1_WAVEFORM_PWM_PHASE_CORRECT_ICR1,
    TIMER1_WAVEFORM_PWM_PHASE_CORRECT_OCR1A,
    TIMER1_WAVEFORM_CTC_ICR1,
} Timer1Waveform;

typedef enum {
    TIMER_CLOCK_NONE,
    TIMER_CLOCK_CLK,
    TIMER_CLOCK_CLK_DIV_8,
    TIMER_CLOCK_CLK_DIV_64,
    TIMER_CLOCK_CLK_DIV_256,
    TIMER_CLOCK_CLK_DIV_1024,
    TIMER_CLOCK_EXTERNAL_FALLING,
    TIMER_CLOCK_EXTERNAL_RISING
} TimerClock;

void (*callback_timer1)(void);
ISR(TIMER1_OVF_vect)
{
    if(callback_timer1)
        callback_timer1();
}

void (*callback_timer2)(void);
ISR(TIMER2_OVF_vect)
{
    if(callback_timer2)
        callback_timer2();
}

void timer0_init_as_pwm(void)
{
    // Clear OC0A and 0C0B on compare match
    reg_write_mask( // Control register A, channel A
        &TCCR0A, 0b11000000,
        OUTPUT_COMPARE_MODE_CLEAR << 6
    );
    reg_write_mask( // Control register A, channel B
        &TCCR0A, 0b00110000,
        OUTPUT_COMPARE_MODE_CLEAR << 6
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
    uint8_t clock = TIMER_CLOCK_CLK;
    reg_write_mask(&TCCR0B, 0b00000111, clock);
}

void timer0_set_duty_cycle_a(float duty_cycle)
{
    uint8_t compare_value = (uint8_t)(duty_cycle * 255);
    OCR0A = compare_value;
}

void timer0_set_duty_cycle_b(float duty_cycle)
{
    uint8_t compare_value = (uint8_t)(duty_cycle * 255);
    OCR0B = compare_value;
}

void (*callback_timer0)(void);
uint64_t counter_timer0;
uint64_t counter_max_timer0;
ISR(TIMER0_OVF_vect)
{
    counter_timer0++;
    if (counter_timer0 == counter_max_timer0) {
        if(callback_timer0)
            callback_timer0();
        counter_timer0 = 0;
    }
}

void timer0_init_as_timer_ms(uint32_t ms, void (*callback)(void))
{
    // Enable interrupts
    sei();
    reg_write_bit(&TIMSK0, TOIE0, 1);
    callback_timer0 = callback;

    // Don't use output compare on either channel
    reg_write_mask(&TCCR0A, 0b11000000, 0);
    reg_write_mask(&TCCR0A, 0b00110000, 0);

    // Normal timer/counter waveform
    reg_write_mask(&TCCR0A, 0b00000011, 0);

    uint8_t clock = TIMER_CLOCK_CLK;
    // With this, one clock period = 256/16MHz = 16us
    reg_write_mask(&TCCR0B, 0b00000111, clock);
}

