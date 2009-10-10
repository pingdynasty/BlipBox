#ifndef _DISPLAYMANAGER_H_
#define _DISPLAYMANAGER_H_

#include <stdint.h>

class DisplayManager {
 public:
  /** shifts the led data in the given direction */
  void shift(uint8_t direction);
  void printCharacter(uint8_t* character, uint8_t row, uint8_t col, uint8_t brightness);
  void setCross(uint8_t row, uint8_t col, uint8_t brightness);
  void setBlob(uint8_t row, uint8_t col, uint8_t brightness);
  void setDiagonalCross(uint8_t row, uint8_t col, uint8_t brightness);
  void setStar(uint8_t row, uint8_t col, uint8_t brightness);
  void setSquare(uint8_t row, uint8_t col, uint8_t brightness);
};

#endif /* _DISPLAYMANAGER_H_ */
