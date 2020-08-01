#include "comms/uart.h"
#include <avr/io.h>

void uart_init(void)
{
    uint16_t baud_rate = 9600;
    uint16_t ubbr_value = ((F_CPU/16)/baud_rate) - 1;
    // Set baud rate
    UBRR0H = (uint8_t)(ubbr_value >> 8);
    UBRR0L = (uint8_t)ubbr_value;

    // If using double speed, write 1 here:
    //reg_write_bit(&UCSR0A, UX0A, 0);

    // Enable receiver
    reg_write_bit(&UCSR0B, RXEN0, 1);
    // Enable transmitter
    reg_write_bit(&UCSR0B, TXEN0, 1);

    // USART mode select -> UART
    // reg_write_mask(&UCSR0C, 0b11000000, 0b00 << 6);

    // Parity mode -> Disable
    // reg_write_mask(&UCSR0C, 0b00110000, 0b00 << 4);

    // Stop select bit -> Leave default

    // Character size
    // Use 8-bit, which requires
    reg_write_bit(&UCSR0B, UCSZ02, 0);
    reg_write_bit(&UCSR0C, UCSZ01, 1);
    reg_write_bit(&UCSR0C, UCSZ00, 1);
}

void uart_putchar(char c)
{
    while (!reg_read_bit(&UCSR0A, UDRE0)) {}
    UDR0 = c;
}

void uart_getchar(char c)
{

}
