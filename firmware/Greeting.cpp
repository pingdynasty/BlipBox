#include <stdint.h>
// #include <avr/delay.h>
#include <LedController.h>

void bounce(LedController& leds){
  uint8_t coords[] = {
    0,0, 1,1, 2,2, 3,3, 4,4, 5,5, 6,6, 7,7, 
    8,6, 9,5, 8,4, 7,3, 6,2, 5,1, 4,0, 3,1,
    2,2, 1,3, 0,4, 1,5, 2,6, 3,7, 4,6, 5,5,
    6,4, 7,3, 8,2, 9,1, 8,0, 7,1, 6,2, 5,3,
    4,4
  };
  for(int8_t i=0; i<33; ++i){
//   for(int8_t i=0; i<6; ++i){
    leds.fade(1);
    leds.setCross(coords[i*2], 7 - coords[i*2+1], 0xff);
    delay(80);
  }
}

void doubleCross(LedController& leds){
  for(int8_t i=0; i<8; ++i){
    leds.fade(1);
    leds.setCross(i+2, i, 0xff);
    leds.setCross(i, 7-i, 0xff);
    delay(100);
  }
}

void rain(LedController& leds){
  // greeting
  for(int8_t i=0; i<48; ++i){
    leds.fade(1);
    leds.setCross(i%10, 7-(i%8), 0xff);
    delay(60);
  }
}

void fade(LedController& leds){
  for(int8_t i=0; i<8; ++i){
    delay(160);
    leds.fade(1);
  }    
}

void explode(LedController& leds){
  for(int8_t i=0; i<8; ++i){
    delay(60);
    leds.brighten(1);
  }    
}

void greeting(LedController& leds){
//   rain(leds);
  bounce(leds);
//   doubleCross(leds);
//   fade(leds);
  explode(leds);
}
