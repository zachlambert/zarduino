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

typedef enum {
    INTERRUPT_EXTERNAL_0,
    INTERRUPT_EXTERNAL_1
} InterruptExternal;

void interrupt_pin_add_callback(
    Pin pin,
    void (*callback)(void)
);

void interrupt_external_add_callback(
    InterruptExternal interrupt_external,
    InterruptType interrupt_type,
    void (*callback)(void)
);

#endif
