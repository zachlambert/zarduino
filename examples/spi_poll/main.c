#include "comms/spi.h"
#include "comms/uart.h"
#include "timing/delay.h"

int main(void)
{
    uart_init();

    Pin SS = PIN_SS;
    SPIConfig config = spi_create_config();
    spi_init_master(&config);
    gpio_mode_output(SS);
    while (1) {
        printf("%d\n", spi_read_byte(SS));
        delay(100);
    }
}
