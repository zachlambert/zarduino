#include "zarduino/comms/uart.h"
#include "zarduino/timing/delay.h"
#include "zarduino/core/pins.h"
#include "zarduino/comms/spi.h"

#include <stdint.h>

int main(void)
{
    SPIConfig config = spi_create_config();
    spi_init_master(&config);

    Pin CS = PIN_ARDUINO_D6;
    gpio_mode_output(CS);
    gpio_write(CS, 1);

    uart_init(0);

    // nRF24L01 radio module
    // Write to a register:
    // - Write a command byte = reg_address & 0x1F | 1<<5
    // - Write the data byte
    // Read from a register:
    // - Write a command byte = reg_address & 0x1F
    // - Write a dummy command (NOP = 0xFF), and read the
    //   data byte which gets sent back at the same time

    uint8_t reg_value = 0x03;
    uint8_t reg_address = 0x00;
    uint8_t write_data_in[] = {
        (reg_address & 0x1F) | 1<<5, 
        reg_value
    };
    uint8_t read_data_in[] = {
        (reg_address & 0x1F),
        0xFF
    };
    uint8_t read_data_out[2];

    while (1) {
        spi_transfer_bytes(CS, write_data_in, 0, 2);
        spi_transfer_bytes(CS, read_data_in, read_data_out, 2);
        printf("Status: %d\n", read_data_out[0]);
        printf("Reg Value: %d\n", read_data_out[1]);
        delay(500);
    }
}
