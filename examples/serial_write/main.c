#include <string.h>
#include "comms/uart.h"
#include "timing/delay.h"

int main(void)
{
    uart_init(); 

    char *message = "Hello, world\n";
    while (1) {
        printf("%s", message);
        delay(500);
    }
}
