#ifndef ZARDUINO_CORE_INTERRUPT
#define ZARDUINO_CORE_INTERRUPT

#include "core/pins.h"

// (External interrupts)

typedef enum {
    INTERRUPT_TYPE_LOW = 0b00,
    INTERRUPT_TYPE_ANY = 0b01,
    INTERRUPT_TYPE_FALLING = 0b10,
    INTERRUPT_TYPE_RISING = 0b11,
} InterruptType;

void interrupt_add_callback(IOPin pin, void (*callback)(void));

#endif
