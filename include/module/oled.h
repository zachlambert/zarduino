#ifndef ZARDUINO_MODULE_OLED
#define ZARDUINO_MODULE_OLED

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint8_t i2c_address;
    size_t width;
    size_t height;
} OLEDConfig;

typedef struct {
    size_t col;
    size_t row;
    uint8_t *i2c_data;
    size_t i2c_data_size;
    uint8_t *buffer;
    size_t buffer_size;
} OLEDData;

OLEDConfig oled_create_config(void);
OLEDData oled_create_data(OLEDConfig *config);

void oled_init(OLEDConfig *config);
void oled_putc(OLEDConfig *config, OLEDData *data, char c);
void oled_clear(OLEDData *data);
void oled_update(OLEDConfig *config, OLEDData *data);

#endif
