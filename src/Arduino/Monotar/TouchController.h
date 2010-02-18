#ifndef _TOUCHCONTROLLER_H_
#define _TOUCHCONTROLLER_H_

#include <stdint.h>

class TouchController {

private:

public:
  void init();
  uint16_t check();
  uint16_t getX();
  uint16_t getY();
  uint16_t getZ();
  uint16_t getValue(uint8_t index);
};

#endif /* _TOUCHCONTROLLER_H_ */
