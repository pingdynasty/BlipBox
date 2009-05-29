#ifndef _DIRECTDIGITALCOUNTER_H_
#define _DIRECTDIGITALCOUNTER_H_

#include <stdint.h>
#include "defs.h"

class Counter {

public:

  void init();

  void increment();

  void reset(){
    off();
    pos = 0;
    on();
  }

  inline uint8_t getPosition(){
    return pos;
  }

 private:
  volatile uint8_t pos;

  void on();
  void off();
};

#endif /* _DIRECTDIGITALCOUNTER_H_ */
