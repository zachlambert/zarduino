#ifndef ZARDUINO_COMMS_UART
#define ZARDUINO_COMMS_UART

#include "zarduino/core/pins.h"
#include "zarduino/core/regs.h"

#include <stdio.h>

typedef enum {
    UART_MODE_ASYNCHRONOUS = 0b00,
    UART_MODE_SYNCHRONOUS = 0b01,
    UART_MODE_MASTER_SPI = 0b11
} UartMode;

typedef enum {
    UART_PARITY_DISABLED = 0b00,
    UART_PARITY_EVEN = 0b10,
    UART_PARITY_ODD = 0b11
} UartParity;

typedef enum {
    UART_STOP_BITS_1,
    UART_STOP_BITS_2
} UartStopBits;

typedef enum {
    UART_BITS_5 = 0b000,
    UART_BITS_6 = 0b001,
    UART_BITS_7 = 0b010,
    UART_BITS_8 = 0b011,
    UART_BITS_9 = 0b111
} UartBits;

typedef struct {
    uint16_t baud_rate;
    UartMode mode;
    UartParity parity;
    UartStopBits stop_bits;
    UartBits bits;
} UartConfig;

UartConfig uart_create_config(void);
void uart_init(UartConfig *config);

void uart_write_byte(uint8_t data);
uint8_t uart_read_byte(void);
void uart_write_bytes(uint8_t *data_in, size_t n);
void uart_read_bytes(uint8_t *data_out, size_t n);

void uart_write_uint16(uint16_t value);
uint16_t uart_read_uint16(void);
void uart_write_int16(int16_t value);
int16_t uart_read_int16(void);

void uart_write_uint32(uint32_t value);
uint32_t uart_read_uint32(void);
void uart_write_int32(int32_t value);
int32_t uart_read_int32(void);

#endif
