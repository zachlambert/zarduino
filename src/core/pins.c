#include "core/pins.h"

#include <avr/io.h>

// To use the GPIO pins, you need to write to the relevant
// registers to configure the direction (input / output) and
// write data (write to the PORTX registers), or read data
// (read from the PINX register).
// These registers are mapped to specific regions in memory,
// identified with DDRA, DDRB, etc.
// To write to the registers, write to these specific pieces
// of memory, using pointers

volatile uint8_t *const io_direction_regs[] = {
    &DDRB, &DDRC, &DDRD
};
volatile uint8_t *const io_output_regs[] = {
    &PORTB, &PORTC, &PORTD
};
volatile uint8_t *const io_input_regs[] = {
    &PINB, &PINC, &PIND
};

IOPin make_io_pin(IOPort port, uint8_t bit)
{
    IOPin pin = {
        .direction_reg = io_direction_regs[port],
        .output_reg = io_output_regs[port],
        .input_reg = io_input_regs[port],
        .bit = bit
    };
    return pin;
}

void reg_write_bit(volatile uint8_t *reg, uint8_t bit, uint8_t value)
{
    if (value) {
        *reg |= 1<<bit;
    } else {
        *reg &= ~(1<<bit);
    }
}

uint8_t reg_read_bit(volatile uint8_t *reg, uint8_t bit)
{
    return (*reg >> bit) & 0x01;
}

void gpio_mode_input(IOPin pin)
{
    reg_write_bit(pin.output_reg, pin.bit, 0);
    reg_write_bit(pin.direction_reg, pin.bit, 0);
}

void gpio_mode_input_pullup(IOPin pin)
{
    // Disabled if PUD flag is set (pull-up disable)
    reg_write_bit(pin.output_reg, pin.bit, 1);
    reg_write_bit(pin.direction_reg, pin.bit, 0);
}

void gpio_mode_output(IOPin pin)
{
    // Sourcing input
    // When the bit is set, the output is pulled high
    reg_write_bit(pin.output_reg, pin.bit, 1);
    reg_write_bit(pin.direction_reg, pin.bit, 1);
}

void gpio_mode_output_inverse(IOPin pin)
{
    // Sinking input
    // When the bit is set, the output is pulled low
    reg_write_bit(pin.output_reg, pin.bit, 0);
    reg_write_bit(pin.direction_reg, pin.bit, 1);
}

void gpio_write(IOPin pin, uint8_t value)
{
    reg_write_bit(pin.output_reg, pin.bit, value);
}

uint8_t gpio_read(IOPin pin)
{
    return reg_read_bit(pin.input_reg, pin.bit);
}

// TODO Put all the pins here
// ...
const IOPin ArduinoPin_BUILT_IN_LED = {
    .direction_reg = &DDRB,
    .output_reg = &PORTB,
    .input_reg = &PINB,
    .bit = 5
};
