#include "core/pins.h"

#include <avr/io.h>

#include "core/regs.h"


typedef struct {
    volatile uint8_t *direction_reg; // DDRX
    volatile uint8_t *output_reg; // PORTX
    volatile uint8_t *input_reg; // PINX
    uint8_t bit;
    uint8_t interrupt_num;
} PinData;


void gpio_mode_input(Pin pin)
{
    PinData *data = (PinData*)pin;
    reg_write_bit(data->output_reg, data->bit, 0);
    reg_write_bit(data->direction_reg, data->bit, 0);
}

void gpio_mode_input_pullup(Pin pin)
{
    PinData *data = (PinData*)pin;
    // Disabled if PUD flag is set (pull-up disable)
    reg_write_bit(data->output_reg, data->bit, 1);
    reg_write_bit(data->direction_reg, data->bit, 0);
}

void gpio_mode_output(Pin pin)
{
    PinData *data = (PinData*)pin;
    // Sourcing input
    // When the bit is set, the output is pulled high
    reg_write_bit(data->output_reg, data->bit, 1);
    reg_write_bit(data->direction_reg, data->bit, 1);
}

void gpio_mode_output_inverse(Pin pin)
{
    PinData *data = (PinData*)pin;
    // Sinking input
    // When the bit is set, the output is pulled low
    reg_write_bit(data->output_reg, data->bit, 0);
    reg_write_bit(data->direction_reg, data->bit, 1);
}

void gpio_write(Pin pin, uint8_t value)
{
    PinData *data = (PinData*)pin;
    reg_write_bit(data->output_reg, data->bit, value);
}

uint8_t gpio_read(Pin pin)
{
    PinData *data = (PinData*)pin;
    return reg_read_bit(data->input_reg, data->bit);
}

// TODO Put all the pins here
PinData PIN_D0_data = {
    .direction_reg = &DDRD,
    .output_reg = &PORTD,
    .input_reg = &PIND,
    .bit = 0
};
const Pin PIN_D0 = &PIN_D0_data;

PinData PIN_D1_data = {
    .direction_reg = &DDRD,
    .output_reg = &PORTD,
    .input_reg = &PIND,
    .bit = 1
};
const Pin PIN_D1 = &PIN_D1_data;

PinData PIN_D2_data = {
    .direction_reg = &DDRD,
    .output_reg = &PORTD,
    .input_reg = &PIND,
    .bit = 2
};
const Pin PIN_D2 = &PIN_D2_data;

PinData PIN_D3_data = {
    .direction_reg = &DDRD,
    .output_reg = &PORTD,
    .input_reg = &PIND,
    .bit = 3
};
const Pin PIN_D3 = &PIN_D3_data;

PinData PIN_D4_data = {
    .direction_reg = &DDRD,
    .output_reg = &PORTD,
    .input_reg = &PIND,
    .bit = 4
};
const Pin PIN_D4 = &PIN_D4_data;

PinData PIN_D5_data = {
    .direction_reg = &DDRD,
    .output_reg = &PORTD,
    .input_reg = &PIND,
    .bit = 5
};
const Pin PIN_D5 = &PIN_D5_data;

PinData PIN_D6_data = {
    .direction_reg = &DDRD,
    .output_reg = &PORTD,
    .input_reg = &PIND,
    .bit = 6
};
const Pin PIN_D6 = &PIN_D6_data;

PinData PIN_D7_data = {
    .direction_reg = &DDRD,
    .output_reg = &PORTD,
    .input_reg = &PIND,
    .bit = 7
};
const Pin PIN_D7 = &PIN_D7_data;

PinData PIN_D8_data = {
    .direction_reg = &DDRB,
    .output_reg = &PORTB,
    .input_reg = &PINB,
    .bit = 0
};
const Pin PIN_D8 = &PIN_D8_data;

PinData PIN_D9_data = {
    .direction_reg = &DDRB,
    .output_reg = &PORTB,
    .input_reg = &PINB,
    .bit = 1
};
const Pin PIN_D9 = &PIN_D9_data;

PinData PIN_D10_data = {
    .direction_reg = &DDRB,
    .output_reg = &PORTB,
    .input_reg = &PINB,
    .bit = 2
};
const Pin PIN_D10 = &PIN_D10_data;

PinData PIN_D11_data = {
    .direction_reg = &DDRB,
    .output_reg = &PORTB,
    .input_reg = &PINB,
    .bit = 3
};
const Pin PIN_D11 = &PIN_D11_data;

PinData PIN_D12_data = {
    .direction_reg = &DDRB,
    .output_reg = &PORTB,
    .input_reg = &PINB,
    .bit = 4
};
const Pin PIN_D12 = &PIN_D12_data;

PinData PIN_D13_data = {
    .direction_reg = &DDRB,
    .output_reg = &PORTB,
    .input_reg = &PINB,
    .bit = 5
};
const Pin PIN_D13 = &PIN_D13_data;

PinData PIN_A0_data = {
    .direction_reg = &DDRC,
    .output_reg = &PORTC,
    .input_reg = &PINC,
    .bit = 0
};
const Pin PIN_A0 = &PIN_A0_data;

PinData PIN_A1_data = {
    .direction_reg = &DDRC,
    .output_reg = &PORTC,
    .input_reg = &PINC,
    .bit = 1
};
const Pin PIN_A1 = &PIN_A1_data;
k
PinData PIN_A2_data = {
    .direction_reg = &DDRC,
    .output_reg = &PORTC,
    .input_reg = &PINC,
    .bit = 2
};
const Pin PIN_A2 = &PIN_A2_data;

PinData PIN_A3_data = {
    .direction_reg = &DDRC,
    .output_reg = &PORTC,
    .input_reg = &PINC,
    .bit = 3
};
const Pin PIN_A3 = &PIN_A3_data;

PinData PIN_A4_data = {
    .direction_reg = &DDRC,
    .output_reg = &PORTC,
    .input_reg = &PINC,
    .bit = 4
};
const Pin PIN_A4 = &PIN_A4_data;

PinData PIN_A5_data = {
    .direction_reg = &DDRC,
    .output_reg = &PORTC,
    .input_reg = &PINC,
    .bit = 5
};
const Pin PIN_A5 = &PIN_A5_data;
