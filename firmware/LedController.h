#ifndef _LEDCONTROLLER_H_
#define _LEDCONTROLLER_H_

#include <stdint.h>
#include "defs.h"
#include "device.h"

#define LED_CHANNELS 16 // 16 for serial connection
#define LED_STRIPS 5

/* #include "JohnsonCounter.h" */
#include "DirectCounter.h"
/* #include "DirectDigitalCounter.h" */

class LedController {
public:
  void init();

  void clear(){
    for(uint8_t row=0; row<LED_STRIPS; ++row)
      for(uint8_t col=0; col<LED_CHANNELS; ++col)
        values[row][col] = 0;
  }

  void displayCurrentRow();

  // row goes from 0-9, col from 0-7
  inline void setLed(uint8_t row, uint8_t col, uint8_t value){
    if(row > 9 || col > 7)
      return;
    if(row < LED_STRIPS)
      values[row][col] = value;
    else
      values[row - LED_STRIPS][col + 8] = value;
//     values[0][row * LED_CHANNELS + col] = value;
  }
  inline uint8_t getLed(uint8_t row, uint8_t col){
    if(row > 9 || col > 7)
      return 0;
    if(row < LED_STRIPS)
      return values[row][col];
    return values[row - LED_STRIPS][col + 8];
//     return values[0][row * LED_CHANNELS + col];
  }
//   // shifts the led data in the given direction
  void shift(uint8_t direction);
  void printCharacter(uint8_t* character, uint8_t row, uint8_t col, uint8_t brightness);
  void brighten(uint8_t factor){
//     for(uint8_t i=0; i<LED_STRIPS*LED_CHANNELS; ++i)
//       values[0][i] <<= factor;
    for(uint8_t row=0; row<LED_STRIPS; ++row)
      for(uint8_t col=0; col<LED_CHANNELS; ++col)
        values[row][col] <<= factor;
  }
  void LedController::fade(uint8_t factor){
    for(uint8_t row=0; row<LED_STRIPS; ++row)
      for(uint8_t col=0; col<LED_CHANNELS; ++col)
        values[row][col] >>= factor;
  }

//   void fade(uint8_t factor);
//   void brighten(uint8_t factor);
  void setCross(uint8_t row, uint8_t col, uint8_t value);
  void setBlob(uint8_t row, uint8_t col, uint8_t value);
  void setDiagonalCross(uint8_t row, uint8_t col, uint8_t value);
  void setStar(uint8_t row, uint8_t col, uint8_t value);
  void setSquare(uint8_t row, uint8_t col, uint8_t value);

#ifdef TLC_VPRG_PIN
  void setGlobalDotCorrection(uint8_t dcval);
#endif

private:
  uint8_t values[LED_STRIPS][LED_CHANNELS];
  Counter counter;
//   LedRow leds[ROWS];
  bool doExtraPulse;
};

#endif /* _LEDCONTROLLER_H_ */
