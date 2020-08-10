#include "module/radio.h"
#include "core/pins.h"
#include "comms/uart.h"
#include "comms/spi.h"
#include <stdint.h>
#include "timing/delay.h"

int main(void)
{
    uart_init();

    SPIConfig spi_config = spi_create_config();
    spi_init_master(&spi_config);

    Pin SS = PIN_SS;
    gpio_write(SS, 0);
    gpio_mode_output(SS);

    while (1) {
        printf("A\n");
        gpio_write(SS, 0);
        spi_transfer(0x00 | 1<<5);
        spi_transfer(0x03);
        gpio_write(SS, 1);

        printf("B\n");
        delay(50);

        gpio_write(SS, 0);
        spi_transfer(0x00);
        uint8_t result = spi_transfer(0xFF);
        gpio_write(SS, 1);

        printf("%x\n", result);

        printf("C\n");
        delay(500);
    }

    // RadioConfig radio_config = radio_create_config();
    // radio_config.CE = PIN_D10;
    // radio_config.CSN = PIN_D8;
    // radio_config.IRQ = PIN_D9;
    // radio_init(&radio_config);

    // uint8_t value_in = 1<<0 | 1<<1;

    // radio_register_write(&radio_config, 0x00, value_in);
    // uint8_t result = radio_register_read(&radio_config, 0x00);
    // printf("%d\n", result);
    // while (1) {

    // }
}
