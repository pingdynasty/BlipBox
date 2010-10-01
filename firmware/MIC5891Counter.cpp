#include "Counter.h"
#include <stdlib.h>
#include <avr/io.h>
#include <wiring.h>
#include "device.h"

#define CNT_PINS 5

void Counter::init(){
  // set DATA and CLOCK lines as outputs
  MIC_DATA_DDR |= _BV(MIC_DATA_PIN);
  MIC_CLOCK_DDR |= _BV(MIC_CLOCK_PIN);
}

void Counter::tick(){
  // increment counter and set data
  if(++pos == CNT_PINS){
    // push out a 1
    MIC_DATA_PORT |= _BV(MIC_DATA_PIN);
    pos = 0;
  }else{
    MIC_DATA_PORT &= ~_BV(MIC_DATA_PIN);
  }
  // toggle clock
  MIC_CLOCK_PORT |= _BV(MIC_CLOCK_PIN);
  MIC_CLOCK_PORT &= ~_BV(MIC_CLOCK_PIN);
}
