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
    radio_config.CSN = PIN_D6;
    radio_config.CE = PIN_D8;
    radio_config.IRQ = 0;

    radio_init(&radio_config);
    radio_set_mode_tx(&radio_config);

    delay(10);
    radio_start(&radio_config);

    uint8_t data_out = 0x55;
    while (1) {
        printf("Sending %d\n", data_out);
        radio_write_tx(&radio_config, data_out);
        delay(100);
    }
}
