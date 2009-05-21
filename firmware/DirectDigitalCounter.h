#ifndef _DIRECTDIGITALCOUNTER_H_
#define _DIRECTDIGITALCOUNTER_H_

#include <stdint.h>
#include "defs.h"

class Counter {

public:

  void init();

  void increment();

  void reset();

  inline uint8_t getCurrentRow(){
    return pos % ROWS;
  }

 private:
  volatile uint8_t pos;
  
};

#endif /* _DIRECTDIGITALCOUNTER_H_ */
