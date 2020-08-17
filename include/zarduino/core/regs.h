#ifndef ZARDUINO_CORE_REGS
#define ZARDUINO_CORE_REGS

#include <stdint.h>

void reg_write_bit(volatile uint8_t *reg, uint8_t bit, uint8_t value);
uint8_t reg_read_bit(volatile uint8_t *reg, uint8_t bit);

void reg_write_mask(volatile uint8_t *reg, uint8_t shift, uint8_t mask, uint8_t value);

uint8_t reg_read_mask(volatile uint8_t *reg, uint8_t shift, uint8_t mask);

#endif
