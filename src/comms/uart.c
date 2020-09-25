#include "zarduino/comms/uart.h"
#include <avr/io.h>
#include <string.h>
#include <stdlib.h>

UartConfig uart_create_config(void)
{
    UartConfig config = {};
    config.baud_rate = 9600;
    config.bits = UART_BITS_8;
    return config;
}

void uart_putchar(char c, FILE *stream)
{
    if (c == '\n') {
        uart_putchar('\r', stream);
    };
    uart_write_byte(c);
}

char uart_getchar(FILE *stream)
{
    return uart_read_byte();
}


void uart_init(UartConfig *config)
{
    UartConfig default_config = uart_create_config();
    if (!config) {
        config = &default_config;
    }

    uint16_t ubbr_value = ((F_CPU/16)/config->baud_rate) - 1;

    // Set baud rate
    UBRR0H = (uint8_t)(ubbr_value >> 8);
    UBRR0L = (uint8_t)ubbr_value;

    // Enable receiver
    reg_write_bit(&UCSR0B, RXEN0, 1);
    // Enable transmitter
    reg_write_bit(&UCSR0B, TXEN0, 1);

    // USART mode
    reg_write_mask(&UCSR0C, UMSEL00, 0b11, config->mode);

    // Parity mode
    reg_write_mask(&UCSR0C, UPM00, 0b11, config->parity);

    // Stop bits
    reg_write_bit(&UCSR0C, USBS0, config->stop_bits);

    // Number of bits
    reg_write_bit(&UCSR0B, UCSZ02, config->bits >> 2);
    reg_write_mask(&UCSR0C, UCSZ00, 0b11, config->bits);

    static FILE uart_ostream = FDEV_SETUP_STREAM(
        uart_putchar, NULL, _FDEV_SETUP_WRITE
    );
    static FILE uart_istream = FDEV_SETUP_STREAM(
        NULL, uart_getchar, _FDEV_SETUP_READ
    );

    stdout = &uart_ostream;
    stdin = &uart_istream;
}

// Core write and read functions

void uart_write_byte(uint8_t data)
{
    while (!reg_read_bit(&UCSR0A, UDRE0));
    UDR0 = data;
}

uint8_t uart_read_byte(void)
{
    while (!reg_read_bit(&UCSR0A, UDRE0));
    return UDR0;
}

// Write and read arrays of bytes

void uart_write_bytes(uint8_t *data_in, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        uart_write_byte(data_in[i]);
    }
}

void uart_read_bytes(uint8_t *data_out, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        data_out[i] = uart_read_byte();
    }
}

// 16-bit integers

void uart_write_uint16(uint16_t value)
{
    uint8_t data[] = {
        value,
        value>>8
    };
    uart_write_bytes(data, 2);
}

uint16_t uart_read_uint16(void)
{
    uint8_t data[2];
    uart_read_bytes(data, 2);
    uint16_t result = data[0] | (uint16_t)data[1]<<8;
    return result;
}

void uart_write_int16(int16_t value)
{
    uint8_t data[] = {
        value,
        value>>8
    };
    uart_write_bytes(data, 2);
}

int16_t uart_read_int16(void)
{
    uint8_t data[2];
    uart_read_bytes(data, 2);
    int16_t result = data[0] | (uint16_t)data[1]<<8;
    return result;
}

// 32-bit integers

void uart_write_uint32(uint32_t value)
{
    uint8_t data[] = {
        value,
        value>>8,
        value>>16,
        value>>24
    };
    uart_write_bytes(data, 4);
}

uint32_t uart_read_uint32(void)
{
    uint8_t data[4];
    uart_read_bytes(data, 4);
    uint32_t result =
        data[0] |
        (uint32_t)data[1]<<8 |
        (uint32_t)data[2]<<16 |
        (uint32_t)data[3]<<24;
    return result;
}

void uart_write_int32(int32_t value)
{
    uint8_t data[] = {
        value,
        value>>8,
        value>>16,
        value>>24
    };
    uart_write_bytes(data, 4);
}

int32_t uart_read_int32(void)
{
    uint8_t data[4];
    uart_read_bytes(data, 4);
    int32_t result =
        data[0] |
        (uint32_t)data[1]<<8 |
        (uint32_t)data[2]<<16 |
        (uint32_t)data[3]<<24;
    return result;
}
