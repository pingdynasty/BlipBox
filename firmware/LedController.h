#ifndef _LEDCONTROLLER_H_
#define _LEDCONTROLLER_H_

#include <stdint.h>
#include <string.h>
#include "defs.h"
#include "device.h"
#include "Counter.h"

#define LED_STRIPS        5
#define LED_CHANNELS      16
#define LED_COLUMNS       8
#define LED_ROWS          10
#define LED_BUFFER_LENGTH LED_STRIPS * LED_CHANNELS

#define LED_DOUBLE_BUFFERED

class LedController {
public:

  void init();

  void flip();

  void displayCurrentRow();

  // x goes from 0-9, y from 0-7
  // buffer is organised as [16][5] matrix
  // offset=i*16+j, 
  // j=y+(x/5)*8, 
  // i=x%5-(x/5)*5, 
  // offset=(y+x/5*8)*16+y+(x/5)*8
  inline void setLed(uint8_t x, uint8_t y, uint8_t brightness){
    // while(doflip);
    x = (x-x/LED_STRIPS*LED_STRIPS)*LED_CHANNELS+y+x/LED_STRIPS*LED_COLUMNS;
    if(x < LED_BUFFER_LENGTH)
      back_buffer[x] = brightness;
  }

  inline uint8_t getLed(uint8_t x, uint8_t y){
    x = (x-x/LED_STRIPS*LED_STRIPS)*LED_CHANNELS+y+x/LED_STRIPS*LED_COLUMNS;
    if(x < LED_BUFFER_LENGTH)
      return back_buffer[x];
  }

  // xor a single led
  void toggle(uint8_t x, uint8_t y){
    setLed(x, y, getLed(x, y) ^ 0xff);
  }

  void clear(){
    // while(doflip);
    memset(back_buffer, 0, LED_BUFFER_LENGTH);
//     bzero(back_buffer, LED_BUFFER_LENGTH);
  }

  void fill(uint8_t brightness){
    // while(doflip);
    memset(back_buffer, brightness, LED_BUFFER_LENGTH);
  }

  // add value to all leds
  void add(uint8_t value){
    // while(doflip);
    for(uint8_t i = 0; i < LED_BUFFER_LENGTH; ++i)
      if(back_buffer[i] <= 0xff - value) // check for overflow
	back_buffer[i] += value;
  }

  // subtract value from all leds
  void sub(uint8_t value){
    // while(doflip);
    for(uint8_t i = 0; i < LED_BUFFER_LENGTH; ++i)
      if(back_buffer[i] >= value) // check for underflow
	back_buffer[i] -= value;
  }

  // xor all leds
  void toggle(){
    // while(doflip);
    for(uint8_t i = 0; i < LED_BUFFER_LENGTH; ++i)
      back_buffer[i] ^= 0xff;
  }

#ifdef TLC_VPRG_PIN
  void setGlobalDotCorrection(uint8_t dcval);
#endif

private:
  bool flipped;
  uint8_t* volatile back_buffer;
  uint8_t buf1[LED_BUFFER_LENGTH];
  uint8_t buf2[LED_BUFFER_LENGTH];
  void sendBufferData(uint8_t row);
  Counter counter;
};

#endif /* _LEDCONTROLLER_H_ */
