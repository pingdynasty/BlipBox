#ifndef _POSITION_H_
#define _POSITION_H_

#include <stdint.h>

class Position {
public:
  uint16_t x;
  uint16_t y;
  uint8_t row, column;
  Position() : x(0), y(0), row(0), column(0) {}
};

#endif /* _POSITION_H_ */
