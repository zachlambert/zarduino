#include "zarduino/module/oled.h"
#include "module/oled_font.h"
#include "zarduino/comms/i2c.h"
#include "zarduino/comms/uart.h"

#include <string.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define ROW_BUFFER_SIZE 129
size_t oled_col;
size_t oled_row;
uint8_t oled_row_buffer[ROW_BUFFER_SIZE];

OLEDConfig oled_create_config()
{
    OLEDConfig config;
    config.i2c_address = 0x3C;
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
        0x22, 0, 7, // Set page address
        0x21, 0, 127, // Set column address
    };
    i2c_write(config->i2c_address, setup_commands, sizeof(setup_commands));

    oled_row_buffer[0] = 0x40;
    memset(oled_row_buffer + 1, 0, ROW_BUFFER_SIZE-1);
    oled_col = 0;
    oled_row = 0;
    oled_clear(config);
}

void oled_flush(OLEDConfig *config)
{
    i2c_write(config->i2c_address, oled_row_buffer, sizeof(oled_row_buffer));

    oled_col = 0;
    oled_row++;
    // oled_row = 2;
    if (oled_row == SCREEN_HEIGHT/8) {
        oled_row = 0;
    }
    memset(oled_row_buffer+1, 0, ROW_BUFFER_SIZE-1);
}

void oled_putc(OLEDConfig *config, char c)
{
    if (c == '\n') {
        oled_flush(config);
    } else if (c == '\r') {
        oled_col = 0;
    } else {
        oled_get_bitmap(c, oled_row_buffer + 1 + oled_col*8);
        oled_col++;
        if (oled_col == SCREEN_WIDTH / 8) {
            oled_flush(config);
            oled_col = 0;
        }
    }
}

void oled_print_string(OLEDConfig *config, char *string)
{
    while (*string != 0) {
        oled_putc(config, *string);
        string++;
    }
}

void oled_clear(OLEDConfig *config)
{
    if (oled_row==0) return;
    i2c_send_start();
    i2c_send_slave_address(config->i2c_address, 0);
    i2c_send_data(0x40);
    for (size_t i = 0; i < 128*(8-oled_row); i++) {
        i2c_send_data(0x00);
    }
    i2c_send_stop();

    oled_row = 0;
    oled_col = 0;
}
