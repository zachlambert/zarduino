#include <string.h>
#include "comms/uart.h"
#include "timing/delay.h"

int main(void)
{
    // Must have TX0/RX0 connected to a usb->serial converter
    // such as the one on arduino boards
    uart_init(); 
    char *message = "Hello, world\n";
    size_t len = strlen(message);
    while (1) {
        for (size_t i = 0; i < len; i++) {
            uart_putchar(message[i]);
        }
        delay(500);
    }
}
