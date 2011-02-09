#include "Counter.h"
#include <stdlib.h>
#include <avr/io.h>
#include <wiring.h>
#include "device.h"

#define CNT_PINS 5

#define MIC_STROBE_PIN  PB0
#define MIC_STROBE_DDR  DDRB
#define MIC_STROBE_PORT PORTB

void Counter::init(){
  // set DATA and CLOCK lines as outputs
  MIC_DATA_DDR |= _BV(MIC_DATA_PIN);
  MIC_CLOCK_DDR |= _BV(MIC_CLOCK_PIN);

  MIC_STROBE_DDR |= _BV(MIC_STROBE_PIN);

//   MIC_DATA_PORT &= ~_BV(MIC_DATA_PIN);
//   for(uint8_t i=0; i<3; ++i){
//     MIC_CLOCK_PORT |= _BV(MIC_CLOCK_PIN);
//     MIC_CLOCK_PORT &= ~_BV(MIC_CLOCK_PIN);
//   }

//   MIC_DATA_PORT |= _BV(MIC_DATA_PIN);
//   MIC_CLOCK_PORT |= _BV(MIC_CLOCK_PIN);
//   MIC_CLOCK_PORT &= ~_BV(MIC_CLOCK_PIN);

//   MIC_DATA_PORT &= ~_BV(MIC_DATA_PIN);
//   for(uint8_t i=0; i<4; ++i){
//     MIC_CLOCK_PORT |= _BV(MIC_CLOCK_PIN);
//     MIC_CLOCK_PORT &= ~_BV(MIC_CLOCK_PIN);
//   }
}

void Counter::tick(){    
  // shift out 5 bits of data
  for(uint8_t i=0; i<CNT_PINS; ++i){
    if(i == pos)
      MIC_DATA_PORT |= _BV(MIC_DATA_PIN);
    else
      MIC_DATA_PORT &= ~_BV(MIC_DATA_PIN);
    // toggle clock
    MIC_CLOCK_PORT |= _BV(MIC_CLOCK_PIN);
    MIC_CLOCK_PORT &= ~_BV(MIC_CLOCK_PIN);
  }
  // toggle strobe
  MIC_STROBE_PORT |= _BV(MIC_STROBE_PIN);
  MIC_STROBE_PORT &= ~_BV(MIC_STROBE_PIN);

  if(++pos == CNT_PINS)
    pos = 0;

  // increment counter and set data
//   if(++pos == CNT_PINS){
//     // push out a 1
//     MIC_DATA_PORT |= _BV(MIC_DATA_PIN);
//     pos = 0;
//   }else{
//     MIC_DATA_PORT &= ~_BV(MIC_DATA_PIN);
//   }
//   // toggle clock
//   MIC_CLOCK_PORT |= _BV(MIC_CLOCK_PIN);
//   MIC_CLOCK_PORT &= ~_BV(MIC_CLOCK_PIN);
}
