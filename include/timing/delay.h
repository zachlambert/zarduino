#ifndef ZARDUINO_TIMING_DELAY_H
#define ZARDUINO_TIMING_DELAY_H

#include <stdint.h>
#include <util/delay.h>

static inline void delay(uint32_t ms)
{
    _delay_ms(ms);
}
static inline void delay_us(uint32_t us)
{
    _delay_us(us);
}

#endif
