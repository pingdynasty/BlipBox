#ifndef _DISPLAYMANAGER_H_
#define _DISPLAYMANAGER_H_

#include <stdint.h>

class DisplayManager {
 public:
  /** shifts the led data in the given direction */
  void shift(uint8_t direction);
  void printCharacter(char c, int8_t row, int8_t col, uint8_t brightness);
  void setCross(uint8_t row, uint8_t col, uint8_t brightness);
  void setBlob(uint8_t row, uint8_t col, uint8_t brightness);
  void setDiagonalCross(uint8_t row, uint8_t col, uint8_t brightness);
  void setStar(uint8_t row, uint8_t col, uint8_t brightness);
  void setSquare(uint8_t row, uint8_t col, uint8_t brightness);
/*   void fade(int8_t amount); */
};

#endif /* _DISPLAYMANAGER_H_ */
