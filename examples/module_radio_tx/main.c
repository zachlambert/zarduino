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

    RadioConfig radio_config = radio_create_config();
    radio_config.CSN = PIN_D6;
    gpio_mode_output(radio_config.CSN);
    gpio_write(radio_config.CSN, 1);

    radio_config.CE = PIN_D8;
    // CE is active high and puts the device into
    // RX/TX mode
    // Must be low when setting register values

    // Set retries to delay 5, count 15
    write_register(0x04, (5 & 0xf) << 4 | (15 & 0xf));

    // Set data rate to 1MBPS
    uint8_t setup = read_register(0x06);
    setup &= ~(1<<5 | 1<<3);
    txDelay = 85;
    write_register(0x06, setup);

    // Write to registers to configure the device
    // Can only be done in power down or standby modes
    // 0x00: Configuration register
    // - Set PWR_UP -> Power up
    // - Set PRIM_RX -> RX mode (when CE set high)
    // - By default, EN_CRC (bit 3) is set
    uint8_t config_value =
        0 << 0 | // PRIM_RX = 0
        1 << 1 | // PWR_UP
        1 << 3;  // CRC, already set, so don't change
    write_register(0x00, config_value);

    // 0x02: Enable RX addresses
    // - Leave default, data pipe 0 and 1 are enabled

    // 0x03: Pipe address lengths
    // - Leave default, length 5 bits

    // uint8_t tx_address = 0b00001;
    // write_register(0x10, tx_address);
    // 0x0A: Set pipe 0 address
    // - 5 bits, set to 00001

    delay(100);

    // Set CE high
    gpio_write(CE, 1);

    uint8_t data_out = 0x55;
    while (1) {
        printf("Sending %d\n", data_out);
        write_tx_byte(data_out);
        delay(100);
    }
}
