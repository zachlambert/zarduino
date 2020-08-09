#include "comms/spi.h"
#include "comms/uart.h"
#include "timing/delay.h"

int main(void)
{
    uart_init();
    SPIConfig config = spi_create_config();
    spi_init_master(&config);
    while (1) {
        printf("%d\n", spi_read_byte());
        delay(100);
    }
}
