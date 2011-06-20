#ifndef _KEYCONTROLLER_H_
#define _KEYCONTROLLER_H_

#include <stdint.h>
#include "TouchController.h"

enum KeyState {
  PRESSED, DRAGGED, RELEASED, UNCHANGED, DISENGAGED
};

class KeyController : public TouchController {

public:
//   void init();
  KeyState keyscan();
/*   void keychange(uint8_t row, uint8_t col, bool ison); */

  inline uint8_t getColumn(){
    return getPosition().column;
  }

  inline uint8_t getRow(){
    return getPosition().row;
  }

  inline bool isPressed(){
    return pressed;
  }

private:
  void update();
  bool pressed;
};

#endif /* _KEYCONTROLLER_H_ */
