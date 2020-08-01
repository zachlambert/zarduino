#ifndef ZARDUINO_CORE_ADC
#define ZARDUINO_CORE_ADC

#include <stdint.h>

// ADC requires a clock frequency between 50kHz and 200Khz
// in order to achieve the maximum resolution of 10 bits.
// If a higher sample rate is required, the resolution will
// be lower.
// ADC Usage:
// 1. Configure the ADC:
// - Timer prescaling
// -
// 2. Select the analog input
// 3. Set the ADSC bit (ADC start conversion bit)
// (Can also configure it to trigger on an interrupt)

typedef enum {
    ADC_REFERENCE_AREF = 0x00,
    ADC_REFERENCE_AVCC_EXT_CAP = 0x01,
    ADC_REFERENCE_INT_1V1_EXT_CAP = 0x11
} ADCConfigReference;

typedef struct {
    struct mux {
        ADCConfigReference reference : 2;
        uint8_t left_adjust_result : 1;
        uint8_t _reserved : 1;
        uint8_t channel_select : 4;
    };
    struct control {
        uint8_t enable : 1;
        uint8_t start : 1;
        uint8_t auto_trigger_enable : 1;
    };
} ADCConfig;

typedef void (*adc_callback_t)(uint16_t);

void adc_initialise(ADCConfig *adc_config);
uint16_t adc_read_polling(ADCConfig *adc_config);
void adc_read_callback(ADCConfig *adc_config, adc_callback_t adc_callback);

#endif
