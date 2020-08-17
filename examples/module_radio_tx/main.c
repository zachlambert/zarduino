#include "module/radio.h"
#include "core/pins.h"
#include "comms/uart.h"
#include "comms/spi.h"
#include "timing/delay.h"

#include <stdint.h>

int main(void)
{
    uart_init();

    SPIConfig spi_config = spi_create_config();
    spi_init_master(&spi_config);

    RadioConfig radio_config = radio_create_config();
    radio_config.CSN = PIN_ARDUINO_D6;
    radio_config.CE = PIN_ARDUINO_D8;
    radio_config.IRQ = 0;

    radio_config.tx_address = 0xA000000012;

    radio_init_as_transmitter(&radio_config);

    delay(10);
    radio_start(&radio_config);

    uint8_t data_out[5] = {0, 1, 2, 3, 4};
    while (1) {
        printf("Sending ");
        for (size_t i = 0; i < 5; i++)
            printf("%x ", data_out[i]);
        printf("\n");

        radio_write_tx(&radio_config, data_out, 5);

        for (size_t i = 0; i < 5; i++)
            data_out[i]++;
        delay(50);
    }
}
