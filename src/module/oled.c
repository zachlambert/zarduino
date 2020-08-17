#include "zarduino/module/oled.h"
#include "module/oled_font.h"
#include "zarduino/comms/i2c.h"
#include "zarduino/comms/uart.h"

#include <string.h>

OLEDConfig oled_create_config()
{
    OLEDConfig config;
    config.i2c_address = 0x3C;
    config.width = 128;
    config.height = 64;
    return config;
}

OLEDData oled_create_data(OLEDConfig *config)
{
    OLEDData data;
    data.buffer_size = config->width * config->height / 8;
    data.i2c_data_size = data.buffer_size + 1;
    data.i2c_data = malloc(data.i2c_data_size);
    // Malloc may fail if not enough ram, in which case you need
    // to free some space somewhere in the program
    data.i2c_data[0] = 0x40;
    data.buffer = data.i2c_data + 1;
    oled_clear(&data);
    data.col = 0;
    data.row = 0;
    return data;
}

void oled_init(OLEDConfig *config)
{
    static uint8_t setup_commands[] = {
        0x00,
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

void oled_putc(OLEDConfig *config, OLEDData *data, char c)
{
    static uint8_t bitmap[8];
    oled_get_bitmap(c, bitmap);
    for (size_t i = 0; i < 8; i++) {
        data->buffer[
            data->row*config->width
            + data->col*config->height/8
            + i]
        = bitmap[i];
    }
    data->col++;
    if (data->col >= config->width/8) {
        data->col = 0;
        data->row++;
        if (data->row >= config->height/8)
            data->row = 0;
    }
}

void oled_print_string(OLEDConfig *config, OLEDData *data, char *string)
{
    while (*string != 0) {
        if (*string == '\n') {
            data->col = 0;
            data->row++;
            if (data->row >= config->width/8)
                data->row = 0;
        } else if (*string == '\r') {
            data->col = 0;
        } else {
            oled_putc(config, data, *string);
        }
        string++;
    }
}

void oled_clear(OLEDData* data)
{
    memset(data->buffer, 0, data->buffer_size);
    data->col = 0;
    data->row = 0;
}

void oled_update(OLEDConfig *config, OLEDData *data)
{
    static uint8_t i2c_data[] = {
        0x00,
        0x22, 0x00, 0xFF, // Set page address to start at 0x00, end at 0xFF
        // (end address above doesn't seem to do anything)
        0x21, 0x00, 127, // Set column address to start at 0, end at max (width-1)
    };
    i2c_write(config->i2c_address, i2c_data, sizeof(i2c_data));

    i2c_write(config->i2c_address, data->i2c_data, data->i2c_data_size);

}
