#include "modules/radio.h"
#include "comms/spi_master.h"
#include "core/pins.h"

RadioConfig radio_create_config(void);

void radio_init(RadioConfig *config)
{
    gpio_mode_output(config->CE);
    gpio_mode_output(config->CSN);
    gpio_mode_input(config->IRQ);
}

void radio_set_mode_rx(RadioConfig *config)
{
    // bit PWR_UP = 1
    // bit PRIM_RX = 1
    // pin CE = 1
    gpio_write(config->CE, 1);
}

void radio_set_mode_tx(RadioConfig *config)
{

}
