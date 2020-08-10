#include "comms/uart.h"
#include "timing/delay.h"
#include "core/pins.h"
#include "comms/spi.h"

#include <stdint.h>

int main(void)
{
    SPIConfig config = spi_create_config();
    spi_init_master(&config);

    Pin CS = PIN_D6;
    gpio_mode_output(CS);
    gpio_write(CS, 1);

    uart_init();

    // Write a byte, and read whatever is sent back

    int8_t data_in = 0x00;
    uint8_t data_out;
    while (1) {
        data_out = spi_transfer_byte(CS, data_in);
        printf("Data out: %d\n", data_out);
        delay(500);
    }
}
