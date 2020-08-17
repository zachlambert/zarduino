#include "zarduino/core/interrupt.h"
#include "zarduino/core/regs.h"
#include <avr/io.h>
#include <stdlib.h>
#include "zarduino/core/pins.h"
#include "core/pin_data.h"
#include <avr/interrupt.h>

// There are two types of interrupt:
// 1.Dedicated external interrupts:
// - INT0: D2
// - INT1: D3
// - Can configure when the interrupt is triggerred
//   (low level, posedge, negedge, both edges)
// 2. Pin change interrupts
// - Can apply to any pin
// - Is triggerred whenever the pin changes
// - Doesn't have a dedicated ISR vector per pin, so have
//   to check the pin value

typedef void (*InterruptCallback)(void);

InterruptCallback callback_int0 = 0;
ISR(INT0_vect)
{
    if (callback_int0)
        callback_int0();
}

InterruptCallback callback_int1 = 0;
ISR(INT1_vect)
{
    if (callback_int1)
        callback_int1();
}

// Corresponds to pins on port B
void (*callbacks_pcint0[8])(void);
uint8_t PINB_prev;
uint8_t PINB_changes;
ISR(PCINT0_vect)
{
    PINB_changes = PINB^PINB_prev; // bitwise XOR
    PINB_prev = PINB;
    for(size_t i=0; i<8; i++) {
        if (callbacks_pcint0[i] && (PINB_changes & 1<<i)) {
            callbacks_pcint0[i]();
        }
    }
}

// Corresponds to pins on port C
void (*callbacks_pcint1[8])(void);
uint8_t PINC_prev;
uint8_t PINC_changes;
ISR(PCINT1_vect)
{
    PINC_changes = PINC^PINC_prev; // bitwise XOR
    PINC_prev = PINC;
    for(size_t i=0; i<8; i++) {
        if (callbacks_pcint1[i] && (PINC_changes & 1<<i)) {
            callbacks_pcint1[i]();
        }
    }
}

// Corresponds to pins on port D
void (*callbacks_pcint2[8])(void);
uint8_t PIND_prev;
uint8_t PIND_changes;
ISR(PCINT2_vect)
{
    PIND_changes = PIND^PIND_prev; // bitwise XOR
    PIND_prev = PIND;
    for(size_t i=0; i<8; i++) {
        if (callbacks_pcint2[i] && (PIND_changes & 1<<i)) {
            callbacks_pcint2[i]();
        }
    }
}

void interrupt_pin_add_callback(Pin pin, void (*callback)(void))
{
    sei();
    PinData *data = pin;
    if (data->pcint < 8) {
        if (!reg_read_bit(&PCICR, PCIE0)) {
            PINB_prev = PINB;
            reg_write_bit(&PCICR, PCIE0, 1);
        }
        callbacks_pcint0[data->bit] = callback;
    } else if (data->pcint < 16) {
        if (!reg_read_bit(&PCICR, PCIE1)) {
            PINC_prev = PINC;
            reg_write_bit(&PCICR, PCIE1, 1);
        }
        callbacks_pcint1[data->bit] = callback;
    } else {
        if (!reg_read_bit(&PCICR, PCIE2)) {
            PIND_prev = PIND;
            reg_write_bit(&PCICR, PCIE2, 1);
        }
        callbacks_pcint2[data->bit] = callback;
    }
    reg_write_bit(&PCMSK0, data->pcint, 1);
}

void interrupt_external_add_callback(
    InterruptExternal interrupt_external,
    InterruptType interrupt_type,
    void (*callback)(void))
{
    sei();
    if (interrupt_external == INTERRUPT_EXTERNAL_0) {
        reg_write_mask(&EICRA, 0, 0b11, interrupt_type);
        reg_write_bit(&EIMSK, INT0, 1); // Enable INT0
        callback_int0 = callback;
    } else {
        reg_write_mask(&EICRA, 2, 0b11, interrupt_type);
        reg_write_bit(&EIMSK, INT1, 1); // Enable INT0
        callback_int1 = callback;
    }
}
