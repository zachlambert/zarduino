#ifndef CORE_PINS_H
#define CORE_PINS_H

#include <stdint.h>

typedef enum {
    IOPort_B,
    IOPort_C,
    IOPort_D,
    IOPort_COUNT
} IOPort;

typedef struct {
    volatile uint8_t *direction_reg; // DDRX
    volatile uint8_t *output_reg; // PORTX
    volatile uint8_t *input_reg; // PINX
    uint8_t bit;
} IOPin;

IOPin make_io_pin(IOPort port, uint8_t bit);

void gpio_mode_input(IOPin pin);
void gpio_mode_input_pullup(IOPin pin);
void gpio_mode_output(IOPin pin);
void gpio_mode_output_inverse(IOPin pin);

void gpio_write(IOPin pin, uint8_t value);
uint8_t gpio_read(IOPin pin);

extern const IOPin PIN_D0;
extern const IOPin PIN_D1;
extern const IOPin PIN_D2;
extern const IOPin PIN_D3;
extern const IOPin PIN_D4;
extern const IOPin PIN_D5;
extern const IOPin PIN_D6;
extern const IOPin PIN_D7;
extern const IOPin PIN_D8;
extern const IOPin PIN_D9;
extern const IOPin PIN_D10;
extern const IOPin PIN_D11;
extern const IOPin PIN_D12;
extern const IOPin PIN_D13;
extern const IOPin PIN_A0;
extern const IOPin PIN_A1;
extern const IOPin PIN_A2;
extern const IOPin PIN_A3;
extern const IOPin PIN_A4;
extern const IOPin PIN_A5;
extern const IOPin PIN_BUILT_IN_LED;

#define PIN_BUILT_IN_LED PIN_D13
#define PIN_SS PIN_D10
#define PIN_MOSI PIN_D11
#define PIN_MISO PIN_D12
#define PIN_SCK PIN_D13
#define PIN_RX PIN_D0
#define PIN_TX PIN_D2

#endif
