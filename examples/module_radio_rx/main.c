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
    radio_set_mode_rx(&radio_config);

    delay(10);
    radio_start(&radio_config);

    uint8_t value = 0xFF;
    while (1) {
        value = radio_read_rx(&radio_config);
        printf("Received %x\n", value);
        delay(50);
    }
}
