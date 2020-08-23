#include "zarduino/module/radio.h"
#include "zarduino/core/pins.h"
#include "zarduino/comms/uart.h"
#include "zarduino/comms/spi.h"
#include "zarduino/timing/delay.h"

#include <stdint.h>

int main(void)
{
    uart_init();

    SPIConfig spi_config = spi_create_config();
    spi_init_master(&spi_config);

    RadioConfig radio_config = radio_create_config();
    radio_config.CSN = PIN_PB0;//PIN_ARDUINO_D6;
    radio_config.CE = PIN_PD7;//PIN_ARDUINO_D8;
    radio_config.IRQ = 0;

    radio_config.rx_base_address = 0xA0000000;
    radio_config.rx_pipe_addresses[0] = 0x12;
    radio_config.rx_payload_sizes[0] = 5;

    radio_init_as_receiver(&radio_config);
    delay(10);
    radio_start(&radio_config);

    uint8_t data_in[5];
    RadioRxStatus rx_status;
    while (1) {
        rx_status = radio_read_rx(&radio_config, data_in, 5);

        if (rx_status != RADIO_RX_STATUS_NOT_USED &&
            rx_status != RADIO_RX_STATUS_EMPTY)
        {
            printf("Received on pipe %d\n:", rx_status);
            for (size_t i = 0; i < 5; i++)
                printf("%x ", data_in[i]);
            printf("\n");
        }

        delay(10);
    }
}
