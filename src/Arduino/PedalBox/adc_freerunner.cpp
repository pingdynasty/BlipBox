#include "adc_freerunner.h"

#include <avr/interrupt.h> 

uint16_t adc_values[ADC_CHANNELS];

void setup_adc(){
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescalar to 128: 125KHz @ 16MHz (divided by 31: 4KHz)
  //  ADCSRA |= (1 << ADPS2) | (1 << ADPS1); // prescalar: 64
  ADMUX |= (1 << REFS0); // Set ADC reference to AVCC
  ADCSRA |= (1 << ADATE); // Set ADC to Free-Running Mode
  ADCSRA |= (1 << ADEN); // Enable ADC
  ADCSRA |= (1 << ADSC); // Start A2D Conversions
  ADCSRA |= (1 << ADIE); // enable ADC interrupt
}

ISR(ADC_vect)
{
  static uint8_t oldchan;
  uint8_t curchan = ADMUX & 7;

  adc_values[oldchan] = ADCL;
  adc_values[oldchan] |= ADCH << 8;

  oldchan = curchan;

  if(++curchan == ADC_CHANNELS)
    curchan = 0;

  ADMUX = (ADMUX & ~7) | curchan;
}
