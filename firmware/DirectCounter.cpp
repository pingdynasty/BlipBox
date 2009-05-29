#include "DirectCounter.h"
#include <stdlib.h>
#include <avr/io.h>
#include <wiring.h>
#include "device.h"

void Counter::init(){
  // Set pins to output
  LED_ROW_1_DDR |= _BV(LED_ROW_1_PIN);
  LED_ROW_2_DDR |= _BV(LED_ROW_2_PIN);
  LED_ROW_3_DDR |= _BV(LED_ROW_3_PIN);
  LED_ROW_4_DDR |= _BV(LED_ROW_4_PIN);
  LED_ROW_5_DDR |= _BV(LED_ROW_5_PIN);
}

void Counter::off(){
  switch(pos){
  case 0:
    LED_ROW_1_PORT &= ~_BV(LED_ROW_1_PIN);
    break;
  case 1:
    LED_ROW_2_PORT &= ~_BV(LED_ROW_2_PIN);
    break;
  case 2:
    LED_ROW_3_PORT &= ~_BV(LED_ROW_3_PIN);
    break;
  case 3:
    LED_ROW_4_PORT &= ~_BV(LED_ROW_4_PIN);
    break;
  case 4:
    LED_ROW_5_PORT &= ~_BV(LED_ROW_5_PIN);
    break;
  }
}

void Counter::on(){
  switch(pos){
  case 0:
    LED_ROW_1_PORT |= _BV(LED_ROW_1_PIN);
    break;
  case 1:
    LED_ROW_2_PORT |= _BV(LED_ROW_2_PIN);
    break;
  case 2:
    LED_ROW_3_PORT |= _BV(LED_ROW_3_PIN);
    break;
  case 3:
    LED_ROW_4_PORT |= _BV(LED_ROW_4_PIN);
    break;
  case 4:
    LED_ROW_5_PORT |= _BV(LED_ROW_5_PIN);
    break;
  }
}  
