#ifndef _LEDCONTROLLER_H_
#define _LEDCONTROLLER_H_

#include <stdint.h>
#include "defs.h"
#include "device.h"

#define LED_STRIPS 5
#define LED_CHANNELS 16 // 16 for serial connection

#define LED_BUFFER_LENGTH LED_STRIPS * LED_CHANNELS

// #include "JohnsonCounter.h"
#include "DirectCounter.h"
/* #include "DirectDigitalCounter.h" */

#include <string.h>

class LedController {
public:
  void init();

  void clear(){
    memset(led_buffer, 0, LED_BUFFER_LENGTH);
  }

  void fill(uint8_t brightness){
    memset(led_buffer, brightness, LED_BUFFER_LENGTH);
  }

  void displayCurrentRow();

  // row goes from 0-9, col from 0-7
  inline void setLed(uint8_t row, uint8_t col, uint8_t brightness){
    if(row > 9 || col > 7)
      return;
    if(row < LED_STRIPS)
      led_buffer[row][col] = brightness;
    else
      led_buffer[row - LED_STRIPS][col + 8] = brightness;
/*     led_buffer[0][row * LED_CHANNELS + col] = brightness; */
  }
  inline uint8_t getLed(uint8_t row, uint8_t col){
    if(row > 9 || col > 7)
      return 0;
    if(row < LED_STRIPS)
      return led_buffer[row][col];
    return led_buffer[row - LED_STRIPS][col + 8];
//     return led_buffer[0][row * LED_CHANNELS + col];
  }
//   // shifts the led data in the given direction
  void shift(uint8_t direction);
  void printCharacter(uint8_t* character, uint8_t row, uint8_t col, uint8_t brightness);
  void brighten(uint8_t factor){
    for(uint8_t i = 0; i < LED_BUFFER_LENGTH; ++i)
      led_buffer[0][i] = (led_buffer[0][i] << factor) | 0x01;
  }
  void fade(uint8_t factor){
    for(uint8_t i = 0; i < LED_BUFFER_LENGTH; ++i)
        led_buffer[0][i] >>= factor;
  }

//   void fade(uint8_t factor);
//   void brighten(uint8_t factor);
  void setCross(uint8_t row, uint8_t col, uint8_t brightness);
  void setBlob(uint8_t row, uint8_t col, uint8_t brightness);
  void setDiagonalCross(uint8_t row, uint8_t col, uint8_t brightness);
  void setStar(uint8_t row, uint8_t col, uint8_t brightness);
  void setSquare(uint8_t row, uint8_t col, uint8_t brightness);

#ifdef TLC_VPRG_PIN
  void setGlobalDotCorrection(uint8_t dcval);
#endif

private:
  uint8_t led_buffer[LED_STRIPS][LED_CHANNELS];
  Counter counter;
//   LedRow leds[ROWS];
  bool doExtraPulse;
};

#endif /* _LEDCONTROLLER_H_ */
