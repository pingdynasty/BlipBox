#ifndef _LEDMATRIX_H_
#define _LEDMATRIX_H_

#include <stdint.h>
#include "DirectCounter.h"

#define LED_CHANNELS 16 // 16 for serial connection
#define LED_STRIPS 5

class LedMatrix {
private:
  uint8_t values[LED_STRIPS][LED_CHANNELS];

 public:
  Counter counter;
  void init();
  uint8_t update();
/*   void setLed(uint8_t index, uint8_t value){ */
/*     values[index] = value; */
/*   } */
  void setLed(uint8_t row, uint8_t col, uint8_t value){
    if(row > 9 || col > 15)
      return;
/*     if(row * LED_CHANNELS + col > LED_CHANNELS * LED_STRIPS) */
/*       return; */
    if(row < LED_STRIPS)
      values[row][col] = value;
    else
      values[row % LED_STRIPS][col + 8] = value;
/*     setLed(row * LED_CHANNELS + col), value); */
/*     values[row % LED_STRIPS][row * LED_CHANNELS + col] = value; */
  }
  void clear(){
    for(uint8_t row=0; row<LED_STRIPS; ++row)
      for(uint8_t col=0; col<LED_CHANNELS; ++col)
        values[row][col] = 0;
  }
};
#endif /* _LEDMATRIX_H_ */
