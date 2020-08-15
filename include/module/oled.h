#ifndef ZARDUINO_MODULE_OLED
#define ZARDUINO_MODULE_OLED

#include <stdint.h>

typedef struct {
    uint8_t i2c_address;
} OLEDConfig;

typedef struct {
    uint8_t buffer[128*64/8];
} OLEDData;

OLEDConfig oled_create_config(void);
void oled_init(OLEDConfig *config);
void oled_putc(OLEDData *data, char c);
void oled_clear(OLEDData *data);
void oled_update(OLEDConfig *config, OLEDData *data);

#endif
