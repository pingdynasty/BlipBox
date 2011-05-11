#ifndef _LEDCONTROLLER_H_
#define _LEDCONTROLLER_H_

#include <stdint.h>
#include "defs.h"
#include "device.h"

#define LED_STRIPS 5
#define LED_CHANNELS 16

#define LED_BUFFER_LENGTH LED_STRIPS * LED_CHANNELS

#include "Counter.h"

#include <string.h>

class LedController {
public:

  void init();
  void start(){
    running = true;
  }
  void stop(){
    running = false;
  }
  uint8_t update();
  void displayCurrentRow();

  // x goes from 0-9, y from 0-7
  inline void setLed(uint8_t x, uint8_t y, uint8_t brightness){
    if(x > 9 || y > 7)
      return;
    if(x < LED_STRIPS)
      led_buffer[x][y] = brightness;
    else
      led_buffer[x - LED_STRIPS][y + 8] = brightness;
  }

  inline uint8_t getLed(uint8_t x, uint8_t y){
    if(x > 9 || y > 7)
      return 0;
    if(x < LED_STRIPS)
      return led_buffer[x][y];
    return led_buffer[x - LED_STRIPS][y + 8];
  }

  // xor a single led
  void toggle(uint8_t x, uint8_t y){
    setLed(x, y, getLed(x, y) ^ 0xff);
  }

  void fill(uint8_t brightness){
    memset(led_buffer, brightness, LED_BUFFER_LENGTH);
  }

  // add value to all leds
  void add(uint8_t value){
    for(uint8_t i = 0; i < LED_BUFFER_LENGTH; ++i)
      if(led_buffer[0][i] <= 0xff - value) // check for overflow
	led_buffer[0][i] += value;
  }

  // subtract value from all leds
  void sub(uint8_t value){
    for(uint8_t i = 0; i < LED_BUFFER_LENGTH; ++i)
      if(led_buffer[0][i] >= value) // check for underflow
	led_buffer[0][i] -= value;
  }

  void brighten(uint8_t factor){
    for(uint8_t i = 0; i < LED_BUFFER_LENGTH; ++i)
      led_buffer[0][i] = (led_buffer[0][i] << factor) | 0x01;
  }

  void fade(uint8_t factor){
    for(uint8_t i = 0; i < LED_BUFFER_LENGTH; ++i)
        led_buffer[0][i] >>= factor;
  }

  // xor all leds
  void toggle(){
    for(uint8_t i = 0; i < LED_BUFFER_LENGTH; ++i)
      led_buffer[0][i] ^= 0xff;
  }

#ifdef TLC_VPRG_PIN
  void setGlobalDotCorrection(uint8_t dcval);
#endif

private:
  void sendBufferData(uint8_t row);
  uint8_t led_buffer[LED_STRIPS][LED_CHANNELS];
  Counter counter;
  bool running;
};

#endif /* _LEDCONTROLLER_H_ */
