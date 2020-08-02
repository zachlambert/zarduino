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

// Array of interrupt callbacks for each pin
void (*interrupt_callbacks[24])(void);
uint32_t prev_pin_values;

ISR(PCINT0_vect)
{
    for(size_t i=0; i<8; i++) {
        if (interrupt_callbacks[i]) {
            // TODO: Read the current pin value, check
            // it has changed
            interrupt_callbacks[i](); 
        }
    }
}

ISR(PCINT1_vect)
{
    for(size_t i=8; i<15; i++) {
        if (interrupt_callbacks[i]) {
            // TODO: Read the current pin value, check
            // it has changed
            interrupt_callbacks[i](); 
        }
    }
}

ISR(PCINT2_vect)
{
    for(size_t i=15; i<24; i++) {
        if (interrupt_callbacks[i]) {
            // TODO: Read the current pin value, check
            // it has changed
            interrupt_callbacks[i](); 
        }
    }
}

void interrupt_add_callback(IOPin pin, void (*callback)(void))
{
    // EICRA: External interrupt control register A
    InterruptType interrupt_type = INTERRUPT_TYPE_FALLING;
    // For INT1
    //reg_write_mask(&EICRA, 0b00001100, interrupt_type<<2);
    // For INT0
    reg_write_mask(&EICRA, 0b00000011, interrupt_type<<2);

    // EIMSK: External interrupt mask register
    // reg_write_bit(&EIMSK, INT1, 1); // Enable INT1
    reg_write_bit(&EIMSK, INT0, 1); // Enable INT0

    /// PCICR: Pin change interrupt control register
    // Select which groups of pins are enabled for interrupts
    // Enable PCINT[23:16], which run the INT2 interrupt
    if (pin.interrupt_num >= 16) {
        reg_write_bit(&PCICR, PCIE, 0);
    }
    // Enable PCINT[23:16], which run the INT1 interrupt
    reg_write_bit(&PCICR, PCIE1, 0);
    // Enable PCINT[23:16], which run the INT0 interrupt
    reg_write_bit(&PCICR, PCIE0, 1);

    // Select the specific pins enabled by a mask
    reg_write_bit(&PCMSK0, pin.interrupt_num, 1);
    // Every pin can trigger interrupts.
    // D2->D7 are INT2->INT7
    // A0->A5 are INT8->INT13
    // etc, configure this later
}
