#ifndef _JOHNSONCOUNTER_H_
#define _JOHNSONCOUNTER_H_

#include <stdint.h>
#include "defs.h"

class Counter {
public:
//   void init() {
//   // initialise 4017 Johnson decade counter
//   // - driven on TLC pins VPRG and XLAT
// //   resetCounter();
//   }

  void init() {}

  void increment();

  void reset();

  inline uint8_t getCurrentRow(){
    return pos;
  }

 private:
  volatile uint8_t pos;
  
};

#endif /* _JOHNSONCOUNTER_H_ */
