#ifndef ZARDUINO_CORE_ADC
#define ZARDUINO_CORE_ADC

#include <stdint.h>
#include "core/pins.h"

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
} ADCReference;

typedef enum {
    ADC_PRESCALER_DIV2 = 0x00,
    // 0x01 also gives DIV2
    ADC_PRESCALER_DIV4 = 0x02,
    ADC_PRESCALER_DIV8 = 0x03,
    ADC_PRESCALER_DIV16 = 0x04,
    ADC_PRESCALER_DIV32 = 0x05,
    ADC_PRESCALER_DIV64 = 0x06,
    ADC_PRESCALER_DIV128 = 0x07
} ADCPrescaler;

typedef enum {
    ADC_AUTO_TRIGGER_SOURCE_FREE_RUNNING_MODE,
    ADC_AUTO_TRIGGER_SOURCE_ANALOG_COMPARATOR,
    ADC_AUTO_TRIGGER_SOURCE_EXTERNAL_INTERRUPT_REQUEST_0,
    ADC_AUTO_TRIGGER_SOURCE_TIMER_0_COMPARE_MATCH_A,
    ADC_AUTO_TRIGGER_SOURCE_TIMER_0_COMPARE_OVERFLOW,
    ADC_AUTO_TRIGGER_SOURCE_TIMER_1_COMPARE_MATCH_B,
    ADC_AUTO_TRIGGER_SOURCE_TIMER_1_COMPARE_OVERFLOW,
    ADC_AUTO_TRIGGER_SOURCE_TIMER_1_CAPTURE_EVENT
} ADCAutoTriggerSource;

typedef struct {
    ADCReference reference;
    ADCPrescaler prescaler_select;
    uint8_t digital_input_disable;
} ADCConfig;

typedef void (*ADCCallback)(uint16_t);

void adc_initialise(ADCConfig *adc_config);
uint16_t adc_read_polling(IOPin pin);
void adc_read_callback(IOPin pin, ADCCallback adc_callback);

#endif
