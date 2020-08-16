#include "module/oled.h"
// #include "module/oled_font.h"
#include "comms/i2c.h"
#include "comms/uart.h"

#include <string.h>

size_t oled_col;
size_t oled_row;
uint8_t oled_buffer[1024+1];

OLEDConfig oled_create_config()
{
    OLEDConfig config = {};
    config.i2c_address = 0x3C;
    config.width = 128;
    config.height = 64;
    return config;
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

void oled_putc(OLEDConfig *config, char c)
{
    // const uint8_t *const bitmap = 0;//oled_get_bitmap(c);
    // // TODO: Change alignment of oled, so bytes are horizontal,
    // // like with the font
    // for (size_t i = 0; i < 8; i++) {
    //     oled_buffer[i] = bitmap[i];
    //     // TODO: Use row and column
    // }
    // oled_col++;
    // if (oled_col == config->width/8) {
    //     oled_col = 0;
    //     oled_row = 0;
    //     if (oled_row == config->height/8)
    //         oled_row = 0;
    // }
    memset(oled_buffer, 0x55, sizeof(oled_buffer));
}

void oled_clear(void)
{
    memset(oled_buffer, 0, sizeof(oled_buffer));
}

void oled_update(OLEDConfig *config)
{
    static uint8_t i2c_data[] = {
        0x00,
        0x22, 0x00, 0xFF, // Set page address to start at 0x00, end at 0xFF
        // (end address above doesn't seem to do anything)
        0x21, 0x00, 127, // Set column address to start at 0, end at max (width-1)
    };
    i2c_write(config->i2c_address, i2c_data, sizeof(i2c_data));
    // uint8_t next_command = 0x40;
    // i2c_write(config->i2c_address, &next_command, 1);
    oled_buffer[0] = 0x40;
    i2c_write(config->i2c_address, oled_buffer, sizeof(oled_buffer));

}
