#include "comms/uart.h"
#include "timing/delay.h"
#include "core/pins.h"
#include "comms/spi.h"

int main(void)
{
    SPIConfig config = spi_create_config();
    spi_init_master(&config);

    Pin CSN = PIN_D6;
    gpio_mode_output(CSN);
    gpio_write(CSN, 1);

    uart_init();

    delay(100);

    while (1) {
        gpio_write(CSN, 0);
        uint8_t read = spi_transfer(0xFF);
        gpio_write(CSN, 1);
        printf("Read: %d\n", read);
        // printf("%d\n", spi_read_byte(SS));
        delay(500);
    }
}
