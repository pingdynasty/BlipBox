#ifndef _POSITION_H_
#define _POSITION_H_

#include <stdint.h>

class Coordinate {
private:
  uint8_t value;

public:
  Coordinate() : value(0) {}
  Coordinate(uint8_t x, uint8_t y)
    : value((x << 4) | (y & 0x0f)) {}

  Coordinate& operator=(const Coordinate& other) {
    value = other.value;
    return *this;
  }

  bool operator<(const Coordinate& other) const {
    return getColumn() < other.getColumn() &&
      getRow() < other.getRow();
  }

//   bool lteq(const Coordinate& other){
  bool operator<=(const Coordinate& other) const {
    return getColumn() <= other.getColumn() &&
      getRow() <= other.getRow();
  }

  bool operator==(const Coordinate& other) const {
    return getColumn() == other.getColumn() &&
      getRow() == other.getRow();
  }

  bool operator!=(const Coordinate& other) const {
    return getColumn() != other.getColumn() &&
      getRow() != other.getRow();
  }

  uint8_t getColumn() const {
    return value >> 4;
  }

  uint8_t getRow() const {
    return value & 0x0f;
  }

  void setColumn(uint8_t x){
    value = (x << 4) | (value & 0x0f);
  }

  void setRow(uint8_t y){
    value = (value & 0xf0) | (y & 0x0f);
  }

  void setValue(uint8_t val){
    value = val;
  }

  uint8_t getValue(){
    return value;
  }
};

class Position : public Coordinate {
public:
  uint16_t x;
  uint16_t y;
 Position() : Coordinate(), x(0), y(0) {}
};

#endif /* _POSITION_H_ */
