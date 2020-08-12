#include "module/radio.h"
#include "core/pins.h"
#include "comms/uart.h"
#include "comms/spi.h"
#include <stdint.h>
#include "timing/delay.h"

Pin CSN;
Pin CE;

void write_register(uint8_t address, uint8_t value)
{
    uint8_t data_in[] = {
        (address & 0x1F) | 1<<5,
        value
    };
    spi_transfer_bytes(CSN, data_in, 0, 2);
}

uint8_t read_register(uint8_t address)
{
    uint8_t data_in[] = {
        (address & 0x1F),
        0xFF
    };
    uint8_t data_out[2];
    spi_transfer_bytes(CSN, data_in, data_out, 2);
    return data_out[1];
}

uint8_t read_rx_byte(void)
{
    uint8_t data_in[] = {
        0b01100001,
        0xFF
    };
    uint8_t data_out[2];
    spi_transfer_bytes(CSN, data_in, data_out, 2);
    return data_out[1];
}

int main(void)
{
    uart_init();

    SPIConfig spi_config = spi_create_config();
    spi_init_master(&spi_config);

    CSN = PIN_D6;
    gpio_mode_output(CSN);
    gpio_write(CSN, 1);

    CE = PIN_D8;
    // CE is active high and puts the device into
    // RX/TX mode
    // Must be low when setting register values

    // Write to registers to configure the device
    // Can only be done in power down or standby modes
    // 0x00: Configuration register
    // - Set PWR_UP -> Power up
    // - Set PRIM_RX -> RX mode (when CE set high)
    // - By default, EN_CRC (bit 3) is set
    uint8_t config_value =
        1 << 0 | // PRIM_RX
        1 << 1 | // PWR_UP
        1 << 3;  // CRC, already set, so don't change
    write_register(0x00, config_value);

    // 0x02: Enable RX addresses
    // - Leave default, data pipe 0 and 1 are enabled

    // 0x03: Pipe address lengths
    // - Leave default, length 5 bits

    // uint8_t rx_pipe0_address = 0b00001;
    // write_register(0x0A, rx_pipe0_address);
    // 0x0A: Set pipe 0 address
    // - 5 bits, set to 00001

    delay(10);

    // Set CE high
    gpio_write(CE, 1);

    // Now, data should be read the RX payload
    // Done using the R_RX_PAYLOAD command
    while (1) {
        uint8_t value = read_rx_byte();
        printf("%x\n", value);
        delay(100);
    }
}
