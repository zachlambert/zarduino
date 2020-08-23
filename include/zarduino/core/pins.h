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

extern const Pin PIN_PB0;
extern const Pin PIN_PB1;
extern const Pin PIN_PB2;
extern const Pin PIN_PB3;
extern const Pin PIN_PB4;
extern const Pin PIN_PB5;
extern const Pin PIN_PB6;
extern const Pin PIN_PB7;

extern const Pin PIN_PC0;
extern const Pin PIN_PC1;
extern const Pin PIN_PC2;
extern const Pin PIN_PC3;
extern const Pin PIN_PC4;
extern const Pin PIN_PC5;
extern const Pin PIN_PC6;
// No PC7 pin

extern const Pin PIN_PD0;
extern const Pin PIN_PD1;
extern const Pin PIN_PD2;
extern const Pin PIN_PD3;
extern const Pin PIN_PD4;
extern const Pin PIN_PD5;
extern const Pin PIN_PD6;
extern const Pin PIN_PD7;


#define PIN_ARDUINO_D0 PIN_PD0
#define PIN_ARDUINO_D1 PIN_PD1
#define PIN_ARDUINO_D2 PIN_PD2
#define PIN_ARDUINO_D3 PIN_PD3
#define PIN_ARDUINO_D4 PIN_PD4
#define PIN_ARDUINO_D5 PIN_PD5
#define PIN_ARDUINO_D6 PIN_PD6
#define PIN_ARDUINO_D7 PIN_PD7
#define PIN_ARDUINO_D8 PIN_PB0
#define PIN_ARDUINO_D9 PIN_PB1
#define PIN_ARDUINO_D10 PIN_PB2
#define PIN_ARDUINO_D11 PIN_PB3
#define PIN_ARDUINO_D12 PIN_PB4
#define PIN_ARDUINO_D13 PIN_PB5

#define PIN_ARDUINO_A0 PIN_PC0
#define PIN_ARDUINO_A1 PIN_PC1
#define PIN_ARDUINO_A2 PIN_PC2
#define PIN_ARDUINO_A3 PIN_PC3
#define PIN_ARDUINO_A4 PIN_PC4
#define PIN_ARDUINO_A5 PIN_PC5

#define PIN_ARDUINO_LED PIN_PB5

#define PIN_SS PIN_PB2
#define PIN_MOSI PIN_PB3
#define PIN_MISO PIN_PB4
#define PIN_SCK PIN_PB5
#define PIN_SCL PIN_PC5
#define PIN_SDA PIN_PC4
#define PIN_RX PIN_PD0
#define PIN_TX PIN_PD1
#define PIN_XTAL1 PIN_PB6
#define PIN_XTAL2 PIN_PB7
#define PIN_RESET PIN_PC6

#define PIN_TIMER0_A PIN_PD6
#define PIN_TIMER0_B PIN_PD5
#define PIN_TIMER1_A PIN_PB1
#define PIN_TIMER1_B PIN_PB2
#define PIN_TIMER2_A PIN_PB3
#define PIN_TIMER2_B PIN_PD3

#define PIN_INT0 PIN_PD2
#define PIN_INT1 PIN_PD3

#endif
