#include "comms/spi.h"
#include "comms/uart.h"
#include "timing/delay.h"

#include <stdint.h>

uint8_t processed;
Pin SS;
void callback(uint8_t data) {
    printf("%d\n", data);
    gpio_write(SS, 1);
    processed = 1;
}

int main(void)
{
    uart_init();

    SS = PIN_SS;
    gpio_mode_output(SS);
    SPIConfig config = spi_create_config();
    config.callback = callback;
    spi_init_master(&config);
    while (1) {
        processed = 0;
        gpio_write(PIN_SS, 0);
        while (!processed);
        delay(10);
    }
}
