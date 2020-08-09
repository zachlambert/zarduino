#ifndef ZARDUINO_MODULES_RADIO
#define ZARDUINO_MODULES_RADIO

// nRF24L01 module

#include "core/pins.h"

typedef struct {
    Pin CE;
    Pin CSN;
    Pin IRQ;
} RadioConfig;

RadioConfig radio_create_config(void);

void radio_init(RadioConfig *config);
void radio_set_mode_rx(RadioConfig *config);
void radio_set_mode_tx(RadioConfig *config);



#endif
