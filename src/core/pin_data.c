#include "core/pins.h"
#include "core/pin_data.h"

#include <avr/io.h>

#include "core/regs.h"

void gpio_mode_input(Pin pin)
{
    if (!pin) return;
    PinData *data = (PinData*)pin;
    reg_write_bit(data->output_reg, data->bit, 0);
    reg_write_bit(data->direction_reg, data->bit, 0);
}

void gpio_mode_input_pullup(Pin pin)
{
    if (!pin) return;
    PinData *data = (PinData*)pin;
    // Disabled if PUD flag is set (pull-up disable)
    reg_write_bit(data->output_reg, data->bit, 1);
    reg_write_bit(data->direction_reg, data->bit, 0);
}

void gpio_mode_output(Pin pin)
{
    if (!pin) return;
    PinData *data = (PinData*)pin;
    // Sourcing input
    // When the bit is set, the output is pulled high
    reg_write_bit(data->output_reg, data->bit, 1);
    reg_write_bit(data->direction_reg, data->bit, 1);
}

void gpio_mode_output_inverse(Pin pin)
{
    if (!pin) return;
    PinData *data = (PinData*)pin;
    // Sinking input
    // When the bit is set, the output is pulled low
    reg_write_bit(data->output_reg, data->bit, 0);
    reg_write_bit(data->direction_reg, data->bit, 1);
}

void gpio_write(Pin pin, uint8_t value)
{
    if (!pin) return;
    PinData *data = (PinData*)pin;
    reg_write_bit(data->output_reg, data->bit, value);
}

uint8_t gpio_read(Pin pin)
{
    if (!pin) return 0x00;
    PinData *data = (PinData*)pin;
    return reg_read_bit(data->input_reg, data->bit);
}

PinData PIN_D0_data = {
    .direction_reg = &DDRD,
    .output_reg = &PORTD,
    .input_reg = &PIND,
    .bit = 0,
    .pcint = 16
};
const Pin PIN_D0 = &PIN_D0_data;

PinData PIN_D1_data = {
    .direction_reg = &DDRD,
    .output_reg = &PORTD,
    .input_reg = &PIND,
    .bit = 1,
    .pcint = 17
};
const Pin PIN_D1 = &PIN_D1_data;

PinData PIN_D2_data = {
    .direction_reg = &DDRD,
    .output_reg = &PORTD,
    .input_reg = &PIND,
    .bit = 2,
    .pcint = 18
};
const Pin PIN_D2 = &PIN_D2_data;

PinData PIN_D3_data = {
    .direction_reg = &DDRD,
    .output_reg = &PORTD,
    .input_reg = &PIND,
    .bit = 3,
    .pcint = 19
};
const Pin PIN_D3 = &PIN_D3_data;

PinData PIN_D4_data = {
    .direction_reg = &DDRD,
    .output_reg = &PORTD,
    .input_reg = &PIND,
    .bit = 4,
    .pcint = 20
};
const Pin PIN_D4 = &PIN_D4_data;

PinData PIN_D5_data = {
    .direction_reg = &DDRD,
    .output_reg = &PORTD,
    .input_reg = &PIND,
    .bit = 5,
    .pcint = 21
};
const Pin PIN_D5 = &PIN_D5_data;

PinData PIN_D6_data = {
    .direction_reg = &DDRD,
    .output_reg = &PORTD,
    .input_reg = &PIND,
    .bit = 6,
    .pcint = 22
};
const Pin PIN_D6 = &PIN_D6_data;

PinData PIN_D7_data = {
    .direction_reg = &DDRD,
    .output_reg = &PORTD,
    .input_reg = &PIND,
    .bit = 7,
    .pcint = 23
};
const Pin PIN_D7 = &PIN_D7_data;

PinData PIN_D8_data = {
    .direction_reg = &DDRB,
    .output_reg = &PORTB,
    .input_reg = &PINB,
    .bit = 0,
    .pcint = 0
};
const Pin PIN_D8 = &PIN_D8_data;

PinData PIN_D9_data = {
    .direction_reg = &DDRB,
    .output_reg = &PORTB,
    .input_reg = &PINB,
    .bit = 1,
    .pcint = 1
};
const Pin PIN_D9 = &PIN_D9_data;

PinData PIN_D10_data = {
    .direction_reg = &DDRB,
    .output_reg = &PORTB,
    .input_reg = &PINB,
    .bit = 2,
    .pcint = 2,
};
const Pin PIN_D10 = &PIN_D10_data;

PinData PIN_D11_data = {
    .direction_reg = &DDRB,
    .output_reg = &PORTB,
    .input_reg = &PINB,
    .bit = 3,
    .pcint = 3
};
const Pin PIN_D11 = &PIN_D11_data;

PinData PIN_D12_data = {
    .direction_reg = &DDRB,
    .output_reg = &PORTB,
    .input_reg = &PINB,
    .bit = 4,
    .pcint = 4
};
const Pin PIN_D12 = &PIN_D12_data;

PinData PIN_D13_data = {
    .direction_reg = &DDRB,
    .output_reg = &PORTB,
    .input_reg = &PINB,
    .bit = 5,
    .pcint = 5
};
const Pin PIN_D13 = &PIN_D13_data;

PinData PIN_A0_data = {
    .direction_reg = &DDRC,
    .output_reg = &PORTC,
    .input_reg = &PINC,
    .bit = 0,
    .pcint = 8
};
const Pin PIN_A0 = &PIN_A0_data;

PinData PIN_A1_data = {
    .direction_reg = &DDRC,
    .output_reg = &PORTC,
    .input_reg = &PINC,
    .bit = 1,
    .pcint = 9
};
const Pin PIN_A1 = &PIN_A1_data;

PinData PIN_A2_data = {
    .direction_reg = &DDRC,
    .output_reg = &PORTC,
    .input_reg = &PINC,
    .bit = 2,
    .pcint = 10
};
const Pin PIN_A2 = &PIN_A2_data;

PinData PIN_A3_data = {
    .direction_reg = &DDRC,
    .output_reg = &PORTC,
    .input_reg = &PINC,
    .bit = 3,
    .pcint = 11
};
const Pin PIN_A3 = &PIN_A3_data;

PinData PIN_A4_data = {
    .direction_reg = &DDRC,
    .output_reg = &PORTC,
    .input_reg = &PINC,
    .bit = 4,
    .pcint = 12
};
const Pin PIN_A4 = &PIN_A4_data;

PinData PIN_A5_data = {
    .direction_reg = &DDRC,
    .output_reg = &PORTC,
    .input_reg = &PINC,
    .bit = 5,
    .pcint = 13
};
const Pin PIN_A5 = &PIN_A5_data;

PinData PIN_XTAL1_data = {
    .direction_reg = &DDRB,
    .output_reg = &PORTB,
    .input_reg = &PINB,
    .bit = 6,
    .pcint = 6
};
const Pin PIN_XTAL1 = &PIN_XTAL1_data;

PinData PIN_XTAL2_data = {
    .direction_reg = &DDRB,
    .output_reg = &PORTB,
    .input_reg = &PINB,
    .bit = 7,
    .pcint = 7
};
const Pin PIN_XTAL2 = &PIN_XTAL2_data;

PinData PIN_RESET_data = {
    .direction_reg = &DDRC,
    .output_reg = &PORTC,
    .input_reg = &PINC,
    .bit = 6,
    .pcint = 14
};
const Pin PIN_RESET = &PIN_RESET_data;
