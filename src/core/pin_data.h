#ifndef ZARDUINO_CORE_PIN_DATA_H
#define ZARDUINO_CORE_PIN_DATA_H

#include <stdint.h>

typedef struct {
    uint8_t port : 2;
    uint8_t bit : 3;
    uint8_t pcint : 5;
    uint8_t adc_pin : 3;
} PinData;

#endif
