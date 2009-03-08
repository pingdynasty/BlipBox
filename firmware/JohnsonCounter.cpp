#include "JohnsonCounter.h"
#include <stdlib.h>
#include <avr/io.h>
#include <wiring.h>

#define CNT_PORT PORTB
#define CNT_PIN PB4 // also counter reset

void Counter::increment(){
  if(++pos == ROWS)
    pos = 0;
}

void Counter::reset(){
  CNT_PORT |= _BV(CNT_PIN);
  CNT_PORT &= ~_BV(CNT_PIN);
  pos = 1;
}
