#ifndef _TOUCHCONTROLLER_H_
#define _TOUCHCONTROLLER_H_

#include <stdint.h>

class TouchController {
private:
public:
  void init();
  uint16_t getValue(uint8_t index);
  uint16_t getZ();
};

#endif /* _TOUCHCONTROLLER_H_ */
