#ifndef ZARDUINO_CORE_PIN_DATA_H
#define ZARDUINO_CORE_PIN_DATA_H

#include <stdint.h>

typedef struct {
    volatile uint8_t *direction_reg; // DDRX
    volatile uint8_t *output_reg; // PORTX
    volatile uint8_t *input_reg; // PINX
    uint8_t bit;
    uint8_t pcint;
} PinData;

#endif
