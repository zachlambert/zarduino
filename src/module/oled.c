#include "module/oled.h"
#include "module/oled_font.h"
#include "comms/i2c.h"

#include <string.h>

OLEDConfig oled_create_config()
{
    OLEDConfig config = {};
    config.i2c_address = 0x1E;
    return config;
}

void oled_init(OLEDConfig *config)
{
    static const uint8_t setup_commands[] = {
        0xAE, // Turn off
        0xD5, 0x80, // Setup clock
        0xA8, 63, // Set panel height
        0xD3, // Display offset
        0x00, // no offset
        0x40, // Start-up line
        0x8D, 0x14, // Configure charge pump, required before turning display on
        0x20, 0x00, // Set memory mode to horizontal addressing
        0xA0 | 0x01, // Configure mapping between data column address and segment driver
        0xC8, // Set COM output scan direction
        0xDA, 0b00010010, // Set COM pins hardware configuration
        0x81, 0xC0, // Set contrast
        0xD9, 0xF1, // Set pre-charge period to phase 1 = 0x1, phase 2 = 0xF
        0xDB, 0x40, // Set vcomh deselect level
        0xA4, // Resume display, with loading content from RAM
        0xA6, // Set normal mode (value 1 -> pixel on)
        0x2E, // Deactivate scroll
        0xAF, // Turn display on physically
    };
    i2c_write(config->i2c_address, setup_commands, sizeof(setup_commands));
}

void oled_putc(OLEDData *data, char c)
{
    uint8_t bitmap = oled_get_bitmap(c);
}

void oled_clear(OLEDData *data)
{
    memset(data->buffer, 0, sizeof(data->buffer));
}

void oled_update(OLEDConfig *config, OLEDData *data)
{
    static const uint8_t write_setup_commands[] = {
        0x22, 0x00, 0xFF, // Set page address to start at 0x00, end at 0xFF
        // (end address above doesn't seem to do anything)
        0x21, 0x00, 127, // Set column address to start at 0, end at max (width-1)
    };
    i2c_write(config->i2c_address, write_setup_commands, sizeof(write_setup_commands));
    uint8_t next_command;
    i2c_write(config->i2c_address, &next_command, 1);
    i2c_write(config->i2c_address, &next_command, 1);
    i2c_write(config->i2c_address, data->buffer, sizeof(data->buffer));
}
