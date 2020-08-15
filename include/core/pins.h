#ifndef CORE_PINS_H
#define CORE_PINS_H

#include <stdint.h>

// Use an opaque handle to pin data
typedef void *Pin;

void gpio_mode_input(Pin pin);
void gpio_mode_input_pullup(Pin pin);
void gpio_mode_output(Pin pin);
void gpio_mode_output_inverse(Pin pin);

void gpio_write(Pin pin, uint8_t value);
uint8_t gpio_read(Pin pin);

extern const Pin PIN_D0;
extern const Pin PIN_D1;
extern const Pin PIN_D2;
extern const Pin PIN_D3;
extern const Pin PIN_D4;
extern const Pin PIN_D5;
extern const Pin PIN_D6;
extern const Pin PIN_D7;
extern const Pin PIN_D8;
extern const Pin PIN_D9;
extern const Pin PIN_D10;
extern const Pin PIN_D11;
extern const Pin PIN_D12;
extern const Pin PIN_D13;
extern const Pin PIN_A0;
extern const Pin PIN_A1;
extern const Pin PIN_A2;
extern const Pin PIN_A3;
extern const Pin PIN_A4;
extern const Pin PIN_A5;
extern const Pin PIN_XTAL1;
extern const Pin PIN_XTAL2;
extern const Pin PIN_RESET;

#define PIN_BUILT_IN_LED PIN_D13

#define PIN_SS PIN_D10
#define PIN_MOSI PIN_D11
#define PIN_MISO PIN_D12
#define PIN_SCK PIN_D13
#define PIN_SCL PIN_A5
#define PIN_SDA PIN_A4
#define PIN_RX PIN_D0
#define PIN_TX PIN_D2

#define PIN_TIMER0_A_D6 PIN_D6
#define PIN_TIMER0_B_D5 PIN_D5
#define PIN_TIMER1_A_D9 PIN_D9
#define PIN_TIMER1_B_D10 PIN_D10
#define PIN_TIMER2_A_D11 PIN_D11
#define PIN_TIMER2_B_D3 PIN_D3

#endif
