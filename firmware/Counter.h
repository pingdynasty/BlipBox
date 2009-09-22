#ifndef _DIRECTCOUNTER_H_
#define _DIRECTCOUNTER_H_

#include <stdint.h>
#include "defs.h"

#define LED_ROW_PINS 5

class Counter {

public:
  void init();

  inline uint8_t getPosition(){
    return pos;
  }

  void increment(){
    this->off();
    if(++pos == LED_ROW_PINS)
      pos = 0;
    this->on();
  }

  void reset(){
    if(pos != 0){
      this->off();
      pos = 0;
      this->on();
    }
  }

 private:
  volatile uint8_t pos;

  // turn current pin on
  void on();
  // turn current pin off
  void off();
  
};

#endif /* _DIRECTCOUNTER_H_ */
