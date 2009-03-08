#include "DirectCounter.h"
#include <stdlib.h>
#include <avr/io.h>
#include <wiring.h>

#define CNT_PORT PORTD
#define CNT_PINS 10 // only 5, really, but we map from 10 values

// note: make sure that CNT_PINS is the right size!
int pins[] = {PD3, PD4, PD5, PD6, PD7, 
              PD3, PD4, PD5, PD6, PD7};

void Counter::init(){
  // Set pins to output
  for(uint8_t i=0; i<CNT_PINS; ++i)
    DDRD |= _BV(pins[i]);
}

void Counter::increment(){
  CNT_PORT &= ~_BV(pins[pos]);
  if(++pos == CNT_PINS)
    pos = 0;
  CNT_PORT |= _BV(pins[pos]);
}

void Counter::reset(){
  CNT_PORT &= ~_BV(pins[pos]);
  pos = 0;
  CNT_PORT |= _BV(pins[pos]);
}
