#ifndef _LEDCONTROLLER_H_
#define _LEDCONTROLLER_H_

#include <stdint.h>
#include "defs.h"

/* #include "JohnsonCounter.h" */
#include "DirectCounter.h"

class LedRow {
private:
  uint8_t values[FRAME_LENGTH];

public:
  void init();
  void clear();
  inline void setLed(uint8_t lednr, uint8_t value){
    values[lednr] = value;
  }

private:
  friend class LedController;
};

class LedController {
public:
  void init();
  void clear();
  void displayCurrentRow();
  void setLed(uint8_t row, uint8_t col, uint8_t value);
  void fade(uint8_t factor);
  void brighten(uint8_t factor);
  void setCross(uint8_t row, uint8_t col, uint8_t value);
  void setBlob(uint8_t row, uint8_t col, uint8_t value);
  void setDiagonalCross(uint8_t row, uint8_t col, uint8_t value);
  void setStar(uint8_t row, uint8_t col, uint8_t value);
  void setSquare(uint8_t row, uint8_t col, uint8_t value);
/*   inline void setLed(uint8_t row, uint8_t col, uint8_t value){ */
/*     leds[row % ROWS].setLed(col, value); */
/*     // manually inlining this method results in a binary exactly the same size */
/*   } */

  void setGlobalDotCorrection(uint8_t dcval);

private:
  Counter counter;
  LedRow leds[ROWS];
  bool doExtraPulse;
};

#endif /* _LEDCONTROLLER_H_ */
