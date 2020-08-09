#include "comms/spi.h"
#include "comms/uart.h"
#include "timing/delay.h"

#include <stdint.h>

void callback(uint8_t data) {
    printf("%d\n", data);
}

int main(void)
{
    uart_init();
    SPIConfig config = spi_create_config();
    config.callback = callback;
    spi_init(&config);
    while (1) {}
}
