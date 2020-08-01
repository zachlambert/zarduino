#include "core/adc.h"
#include "core/regs.h"
#include <avr/io.h>

void adc_initialise(ADCConfig *config)
{
    if (config) {
        reg_write_mask(&ADMUX, 0b11000000, config->reference<<6);
        reg_write_mask(&ADCSRA, 0b00000111, config->prescaler_select);
    } else {
        // Use defaults
        reg_write_mask(&ADMUX, 0b11000000, ADC_REFERENCE_AREF<<6);
        reg_write_mask(&ADCSRA, 0b00000111, ADC_PRESCALER_DIV128);
        // ADC requires a clock frequency between 50kHz and 200Khz
        // in order to achieve the maximum resolution of 10 bits.
        // Only DIV128 satisfies this, with F_CPU = 16MHz
    }
    // Enable ADC
    reg_write_bit(&ADCSRA, ADEN, 1); 
    // Leave left adjust as default, which is right adjusted
    // Therefore, adc result stored in right side of ADCH/ADCL

    // For interrupt enable, etc, configure in separate functions
}

uint16_t adc_read_polling(IOPin pin)
{
    // Disable the digital input
    DIDR0 = 0; // Reset for any previous pins
    reg_write_bit(&DIDR0, pin.bit, 1);
    // Select the corresponding adc input
    // Field has 16 possible sources, but only the bottom 8
    // correspond to an analog input pin
    reg_write_mask(&ADMUX, 0b00000111, pin.bit);
    // Disable interrupts
    reg_write_bit(&ADCSRA, ADIE, 0);
    // Start conversion
    reg_write_bit(&ADCSRA, ADSC, 1);
    // Wait until interrupt flag is set
    while (!reg_read_bit(&ADCSRA, ADIF)) {}
    // Clear the flag, which must be done manually if
    // not using interrupts
    reg_write_bit(&ADCSRA, ADIF, 0);
    // Return result
    return (ADCH << 8 | ADCL);
}

ADCCallback current_adc_callback;
void ISR(ADC_vect)
{
    current_adc_callback(ADCH << 8 | ADCL);
    // Interrupt flag automatically cleared if
    // using interrupts
}

void adc_read_callback(IOPin pin, ADCCallback adc_callback)
{
    // Ensure global interrupts are enabled
    // At bit 7, labelled "I", 
    reg_write_bit(&SREG, 7, 1);
    // Same first steps as above
    DIDR0 = 0;
    reg_write_bit(&DIDR0, pin.bit, 1);
    reg_write_mask(&ADMUX, 0b00000111, pin.bit);
    // Set the adc_callbcak
    current_adc_callback = adc_callback;
    // Enable interrupts
    reg_write_bit(&ADCSRA, ADIE, 1);
    // Start conversion
    reg_write_bit(&ADCSRA, ADSC, 1);
}
