#include "core/adc.h"

void adc_initialise(ADCConfig *adc_config)
{
    // ADMUX - ADC Multiplexer Selection Register
    // REFS1:REFS0 = Select voltage reference
    // 00 -> AREF, invernal Vref off
    // 01 -> AVcc with external capacitor at AREF pin
    // 10 -> Unused
    // 11 -> Internal 1.1V reference with external capacitor
    //       at AREF pin
}

uint16_t adc_read_polling(ADCConfig *adc_config)
{

}

void adc_read_callback(ADCConfig *adc_config, adc_callback_t adc_callback)
{

}
