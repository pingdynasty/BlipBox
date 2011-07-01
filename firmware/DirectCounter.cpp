#include "Counter.h"
#include <avr/io.h>
#include "device.h"

#define LED_ROW_PINS 5

void Counter::init(){
  pos = 0;
  // Set pins to output
  LED_ROW_1_DDR |= _BV(LED_ROW_1_PIN);
  LED_ROW_2_DDR |= _BV(LED_ROW_2_PIN);
  LED_ROW_3_DDR |= _BV(LED_ROW_3_PIN);
  LED_ROW_4_DDR |= _BV(LED_ROW_4_PIN);
  LED_ROW_5_DDR |= _BV(LED_ROW_5_PIN);
}

void Counter::tick(){
  switch(pos){
  case 0:
    LED_ROW_1_PORT &= ~_BV(LED_ROW_1_PIN);
    LED_ROW_2_PORT |= _BV(LED_ROW_2_PIN);
    break;
  case 1:
    LED_ROW_2_PORT &= ~_BV(LED_ROW_2_PIN);
    LED_ROW_3_PORT |= _BV(LED_ROW_3_PIN);
    break;
  case 2:
    LED_ROW_3_PORT &= ~_BV(LED_ROW_3_PIN);
    LED_ROW_4_PORT |= _BV(LED_ROW_4_PIN);
    break;
  case 3:
    LED_ROW_4_PORT &= ~_BV(LED_ROW_4_PIN);
    LED_ROW_5_PORT |= _BV(LED_ROW_5_PIN);
    break;
  case 4:
    LED_ROW_5_PORT &= ~_BV(LED_ROW_5_PIN);
    LED_ROW_1_PORT |= _BV(LED_ROW_1_PIN);
    break;
  }
  if(++pos == LED_ROW_PINS)
    pos = 0;
}
