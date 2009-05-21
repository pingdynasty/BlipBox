#include "DirectCounter.h"
#include <stdlib.h>
#include <avr/io.h>
#include <wiring.h>
#include "device.h"

#define CNT_PINS 10 // only 5, really, but we map from 10 values

// note: make sure that CNT_PINS is the right size!
int pins[] = { LED_ROW_0, LED_ROW_1, LED_ROW_2, LED_ROW_3, LED_ROW_4,
               LED_ROW_0, LED_ROW_1, LED_ROW_2, LED_ROW_3, LED_ROW_4 };

void Counter::init(){
  // Set pins to output
  for(uint8_t i=0; i<CNT_PINS; ++i)
    pinMode(pins[i], OUTPUT);
}

void Counter::increment(){
  digitalWrite(pins[pos], LOW);
  if(++pos == CNT_PINS)
    pos = 0;
  digitalWrite(pins[pos], HIGH);
}

void Counter::reset(){
  digitalWrite(pins[pos], LOW);
  pos = 0;
  digitalWrite(pins[pos], HIGH);
}
