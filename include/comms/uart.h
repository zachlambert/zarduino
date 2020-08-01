#ifndef ZARDUINO_COMMS_UART
#define ZARDUINO_COMMS_UART

#include "core/pins.h"
#include "core/regs.h"

void uart_init(void);
void uart_putchar(char c);
void uart_getchar(char c);

#endif
