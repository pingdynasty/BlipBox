#include "DirectCounter.h"
#include <stdlib.h>
#include <avr/io.h>
#include <wiring.h>
#include "device.h"

#define LED_ROW_PINS 10 // only 5, really, but we map from 10 values

// note: make sure that LED_ROW_PINS is the right size!
int pins[] = { LED_ROW_1, LED_ROW_2, LED_ROW_3, LED_ROW_4, LED_ROW_5,
               LED_ROW_1, LED_ROW_2, LED_ROW_3, LED_ROW_4, LED_ROW_5 };

void Counter::init(){
  // Set pins to output
  for(uint8_t i=0; i<LED_ROW_PINS; ++i)
    LED_ROW_DDR |= _BV(pins[i]);
}

void Counter::increment(){
  LED_ROW_PORT &= ~_BV(pins[pos]);
  if(++pos == LED_ROW_PINS)
    pos = 0;
  LED_ROW_PORT |= _BV(pins[pos]);
}

void Counter::reset(){
  LED_ROW_PORT &= ~_BV(pins[pos]);
  pos = 0;
  LED_ROW_PORT |= _BV(pins[pos]);
}
