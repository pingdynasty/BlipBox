#ifndef _LEDMATRIX_H_
#define _LEDMATRIX_H_

#include <stdint.h>
/* #include "DirectCounter.h" */

#define LED_CHANNELS 16 // 16 for serial connection

/* extern volatile void (*tlc_onUpdateFinished)(void); */

class LedMatrix {
private:
  uint8_t values[LED_CHANNELS];
/*   Counter counter; */

 public:
  void init();
  uint8_t update();
  void set(uint8_t index, uint8_t value){
    values[index] = value;
  }
  void setLed(uint8_t row, uint8_t col, uint8_t value){
    values[col] = value;
  }
  void clear(){
    for(uint8_t i=0; i<LED_CHANNELS; ++i)
      values[i] = 0;
  }
};
#endif /* _LEDMATRIX_H_ */
