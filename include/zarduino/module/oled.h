#ifndef ZARDUINO_MODULE_OLED
#define ZARDUINO_MODULE_OLED

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint8_t i2c_address;
} OLEDConfig;

OLEDConfig oled_create_config(void);

void oled_init(OLEDConfig *config);
void oled_putc(OLEDConfig *config, char c);
void oled_print_string(OLEDConfig *config, char *string);
void oled_clear(OLEDConfig *config);
void oled_flush(OLEDConfig *config);

#endif
