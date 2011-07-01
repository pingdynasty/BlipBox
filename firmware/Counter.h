#ifndef _COUNTER_H_
#define _COUNTER_H_

#include <stdint.h>
#include "defs.h"

class Counter {

public:
  void init();

  inline uint8_t getPosition(){
    return pos;
  }

  void tick();

 private:
  uint8_t pos;  
};

#endif /* _COUNTER_H_ */
