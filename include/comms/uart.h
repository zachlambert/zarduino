#ifndef ZARDUINO_COMMS_UART
#define ZARDUINO_COMMS_UART

#include "core/pins.h"
#include "core/regs.h"

#include <stdio.h>

void uart_init(void);
void uart_putchar(char c, FILE *stream);
char uart_getchar(FILE *stream);

#endif
