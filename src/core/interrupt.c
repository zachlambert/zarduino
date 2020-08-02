#include "core/interrupt.h"
#include "core/regs.h"
#include <avr/io.h>
#include <stdlib.h>

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

InterruptCallback interrupt_callback_int0 = 0;
ISR(INT0_vect)
{
    if (interrupt_callback_int0)
        interrupt_callback_int0();
}

InterruptCallback interrupt_callback_int1 = 0;
ISR(INT1_vect)
{
    if (interrupt_callback_int1)
        interrupt_callback_int1();
}

// Corresponds to pins on port B
void (*pcint0_callbacks[8])(void);
uint8_t PINB_prev;
uint8_t PINB_changes;
ISR(PCINT0_vect)
{
    PINB_changes = PINB^PINB_prev; // bitwise XOR
    PINB_prev = PINB;
    for(size_t i=0; i<8; i++) {
        if (pcint0_callbacks[i] && (PINB_changes & 1<<i)) {
            pcint0_callbacks[i](); 
        }
    }
}

// Corresponds to pins on port C
void (*pcint1_callbacks[8])(void);
uint8_t PINC_prev;
uint8_t PINC_changes;
ISR(PCINT1_vect)
{
    PINC_changes = PINC^PINC_prev; // bitwise XOR
    PINC_prev = PINC;
    for(size_t i=0; i<8; i++) {
        if (pcint1_callbacks[i] && (PINC_changes & 1<<i)) {
            pcint1_callbacks[i](); 
        }
    }
}

// Corresponds to pins on port D
void (*pcint2_callbacks[8])(void);
uint8_t PIND_prev;
uint8_t PIND_changes;
ISR(PCINT2_vect)
{
    PIND_changes = PIND^PIND_prev; // bitwise XOR
    PIND_prev = PIND;
    for(size_t i=0; i<8; i++) {
        if (pcint2_callbacks[i] && (PIND_changes & 1<<i)) {
            pcint2_callbacks[i](); 
        }
    }
}

void interrupt_pin_add_callback(Pin pin, void (*callback)(void))
{
    PinData *data = pin;
    if (pin->pcint < 8) {
        if (!reg_read_bit(&PCICR, PCIE0)) {
            PINB_prev = PINB;
            reg_write_bit(&PCICR, PCIE0, 1);
        }
        pcint0_callbacks[data->bit] = callback;
    } else if (pin->pcint < 16) {
        if (!reg_read_bit(&PCICR, PCIE1)) {
            PINC_prev = PINC;
            reg_write_bit(&PCICR, PCIE1, 1);
        }
        pcint1_callbacks[data->bit] = callback;
    } else {
        if (!reg_read_bit(&PCICR, PCIE2)) {
            PIND_prev = PIND;
            reg_write_bit(&PCICR, PCIE2, 1);
        }
        pcint2_callbacks[data->bit] = callback;
    }
    reg_write_bit(&PCMSK0, data->pcint, 1);
}

void interrupt_external_add_callback(
    InterruptExternal interrupt_external,
    InterruptType interrupt_type,
    void (*callback)(void))
{
    if (interrupt_external == INTERRUPT_EXTERNAL_0) {
        reg_write_mask(&EICRA, 0b00000011, interrupt_type);
        reg_write_bit(&EIMSK, INT0, 1); // Enable INT0
    } else {
        reg_write_mask(&EICRA, 0b00001100, interrupt_type<<2);
        reg_write_bit(&EIMSK, INT1, 1); // Enable INT0
    }
}
