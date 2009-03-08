#ifndef _KEYCONTROLLER_H_
#define _KEYCONTROLLER_H_

#include <stdint.h>
#include "TouchController.h"

class KeyController : public TouchController {

public:
//   void init();
  uint8_t readRow();
  uint8_t readColumn();
  bool keyscan();
  void update();
  void keychange(uint8_t row, uint8_t col, bool ison);

  inline uint8_t getColumn(){
    return col;
  }

  inline uint8_t getRow(){
    return row;
  }

  inline bool isPressed(){
    return pressed;
  }

private:
//   TouchController touch;
  uint8_t col;
  uint8_t row;
  bool pressed;
};

#endif /* _KEYCONTROLLER_H_ */
