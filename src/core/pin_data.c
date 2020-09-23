#include "zarduino/core/pins.h"
#include "core/pin_data.h"

#include <avr/io.h>

#include "zarduino/core/regs.h"

#define PORT_B 0
#define PORT_C 1
#define PORT_D 2
#define ADC_DISABLED 7

volatile uint8_t *direction_reg[3] = {&DDRB, &DDRC, &DDRD};
volatile uint8_t *output_reg[3] = {&PORTB, &PORTC, &PORTD};
volatile uint8_t *input_reg[3] = {&PINB, &PINC, &PIND};

void gpio_mode_input(Pin pin)
{
    PinData *data = (PinData*)pin;
    reg_write_bit(output_reg[data->port], data->bit, 0);
    reg_write_bit(direction_reg[data->port], data->bit, 0);
}

void gpio_mode_input_pullup(Pin pin)
{
    PinData *data = (PinData*)pin;
    // Disabled if PUD flag is set (pull-up disable)
    reg_write_bit(output_reg[data->port], data->bit, 1);
    reg_write_bit(direction_reg[data->port], data->bit, 0);
}

void gpio_mode_output(Pin pin)
{
    PinData *data = (PinData*)pin;
    // Sourcing input
    // When the bit is set, the output is pulled high
    reg_write_bit(output_reg[data->port], data->bit, 1);
    reg_write_bit(direction_reg[data->port], data->bit, 1);
}

void gpio_mode_output_inverse(Pin pin)
{
    PinData *data = (PinData*)pin;
    // Sinking input
    // When the bit is set, the output is pulled low
    reg_write_bit(output_reg[data->port], data->bit, 0);
    reg_write_bit(direction_reg[data->port], data->bit, 1);
}

void gpio_write(Pin pin, uint8_t value)
{
    PinData *data = (PinData*)pin;
    reg_write_bit(output_reg[data->port], data->bit, value);
}

uint8_t gpio_read(Pin pin)
{
    PinData *data = (PinData*)pin;
    return reg_read_bit(input_reg[data->port], data->bit);
}


// === Port B ===

PinData PIN_PB0_data = {
    .port = PORT_B,
    .bit = 0,
    .pcint = 0,
    .adc_pin = ADC_DISABLED
};
const Pin PIN_PB0 = &PIN_PB0_data;

PinData PIN_PB1_data = {
    .port = PORT_B,
    .bit = 1,
    .pcint = 1,
    .adc_pin = ADC_DISABLED
};
const Pin PIN_PB1 = &PIN_PB1_data;

PinData PIN_PB2_data = {
    .port = PORT_B,
    .bit = 2,
    .pcint = 2,
    .adc_pin = ADC_DISABLED
};
const Pin PIN_PB2 = &PIN_PB2_data;

PinData PIN_PB3_data = {
    .port = PORT_B,
    .bit = 3,
    .pcint = 3,
    .adc_pin = ADC_DISABLED
};
const Pin PIN_PB3 = &PIN_PB3_data;

PinData PIN_PB4_data = {
    .port = PORT_B,
    .bit = 4,
    .pcint = 4,
    .adc_pin = ADC_DISABLED
};
const Pin PIN_PB4 = &PIN_PB4_data;

PinData PIN_PB5_data = {
    .port = PORT_B,
    .bit = 5,
    .pcint = 5,
    .adc_pin = ADC_DISABLED
};
const Pin PIN_PB5 = &PIN_PB5_data;

PinData PIN_PB6_data = {
    .port = PORT_B,
    .bit = 6,
    .pcint = 6,
    .adc_pin = ADC_DISABLED
};
const Pin PIN_PB6 = &PIN_PB6_data;

PinData PIN_PB7_data = {
    .port = PORT_B,
    .bit = 7,
    .pcint = 7,
    .adc_pin = ADC_DISABLED
};
const Pin PIN_PB7 = &PIN_PB7_data;


// === PORT C ===

PinData PIN_PC0_data = {
    .port = PORT_C,
    .bit = 0,
    .pcint = 8,
    .adc_pin = 0
};
const Pin PIN_PC0 = &PIN_PC0_data;

PinData PIN_PC1_data = {
    .port = PORT_C,
    .bit = 1,
    .pcint = 9,
    .adc_pin = 1
};
const Pin PIN_PC1 = &PIN_PC1_data;

PinData PIN_PC2_data = {
    .port = PORT_C,
    .bit = 2,
    .pcint = 10,
    .adc_pin = 2
};
const Pin PIN_PC2 = &PIN_PC2_data;

PinData PIN_PC3_data = {
    .port = PORT_C,
    .bit = 3,
    .pcint = 11,
    .adc_pin = 3
};
const Pin PIN_PC3 = &PIN_PC3_data;

PinData PIN_PC4_data = {
    .port = PORT_C,
    .bit = 4,
    .pcint = 12,
    .adc_pin = 4
};
const Pin PIN_PC4 = &PIN_PC4_data;

PinData PIN_PC5_data = {
    .port = PORT_C,
    .bit = 5,
    .pcint = 13,
    .adc_pin = 5
};
const Pin PIN_PC5 = &PIN_PC5_data;

PinData PIN_PC6_data = {
    .port = PORT_C,
    .bit = 6,
    .pcint = 14,
    .adc_pin = ADC_DISABLED
};
const Pin PIN_PC6 = &PIN_PC6_data;

// No PC7 pin


// === Port D ===

PinData PIN_PD0_data = {
    .port = PORT_D,
    .bit = 0,
    .pcint = 16,
    .adc_pin = ADC_DISABLED
};
const Pin PIN_PD0 = &PIN_PD0_data;

PinData PIN_PD1_data = {
    .port = PORT_D,
    .bit = 1,
    .pcint = 17,
    .adc_pin = ADC_DISABLED
};
const Pin PIN_PD1 = &PIN_PD1_data;

PinData PIN_PD2_data = {
    .port = PORT_D,
    .bit = 2,
    .pcint = 18,
    .adc_pin = ADC_DISABLED
};
const Pin PIN_PD2 = &PIN_PD2_data;

PinData PIN_PD3_data = {
    .port = PORT_D,
    .bit = 3,
    .pcint = 19,
    .adc_pin = ADC_DISABLED
};
const Pin PIN_PD3 = &PIN_PD3_data;

PinData PIN_PD4_data = {
    .port = PORT_D,
    .bit = 4,
    .pcint = 20,
    .adc_pin = ADC_DISABLED
};
const Pin PIN_PD4 = &PIN_PD4_data;

PinData PIN_PD5_data = {
    .port = PORT_D,
    .bit = 5,
    .pcint = 21,
    .adc_pin = ADC_DISABLED
};
const Pin PIN_PD5 = &PIN_PD5_data;

PinData PIN_PD6_data = {
    .port = PORT_D,
    .bit = 6,
    .pcint = 22,
    .adc_pin = ADC_DISABLED
};
const Pin PIN_PD6 = &PIN_PD6_data;

PinData PIN_PD7_data = {
    .port = PORT_D,
    .bit = 7,
    .pcint = 23,
    .adc_pin = ADC_DISABLED
};
const Pin PIN_PD7 = &PIN_PD7_data;
