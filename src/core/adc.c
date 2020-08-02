#include "core/adc.h"
#include "core/regs.h"
#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>

ADCConfig adc_create_config(void)
{
    ADCConfig config;
    // Provide defaults
    config.reference = ADC_REFERENCE_AVCC;
    config.prescaler_select = ADC_PRESCALER_DIV128;
    return config;
}

void adc_initialise(ADCConfig *config)
{
    reg_write_mask(&ADMUX, 0b11000000, config->reference<<6);
    reg_write_bit(&ADCSRA, ADEN, 1); 
    reg_write_mask(&ADCSRA, 0b00000111, config->prescaler_select);
    reg_write_bit(&ADMUX, 0b00000111, config->analog_pin);
    // Disable the digital input too
    DIDR0 = 0;
    reg_write_bit(&DIDR0, config->analog_pin, 1);
}

uint16_t adc_read_wait(void)
{
    reg_write_bit(&ADCSRA, ADIE, 0);
    reg_write_bit(&ADCSRA, ADSC, 1);
    while (reg_read_bit(&ADCSRA, ADSC));
    // For some reason, it doesn't work if ADCL and ADCH<<8 are
    // switched below
    return (ADCL | ADCH << 8);
}

ADCCallback current_adc_callback;
bool repeat_callback = false;
ISR(ADC_vect)
{
    current_adc_callback(ADCL | ADCH << 8);
    if (repeat_callback)
        reg_write_bit(&ADCSRA, ADSC, 1);
}

void adc_read_callback_single(ADCCallback adc_callback)
{
    // Set the adc_callback
    current_adc_callback = adc_callback;
    // Enable interrupts
    sei();
    reg_write_bit(&ADCSRA, ADIE, 1);
    // Start conversion
    reg_write_bit(&ADCSRA, ADSC, 1);
}

void adc_read_callback_repeat(ADCCallback adc_callback)
{
    repeat_callback = true;
    adc_read_callback_single(adc_callback);
}

void adc_read_callback_repeat_stop(void)
{
    repeat_callback = false;
}
