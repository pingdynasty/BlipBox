#ifndef _KEYCONTROLLER_H_
#define _KEYCONTROLLER_H_

#include <stdint.h>
#include "TouchController.h"
#include "Position.h"

class KeyController : public TouchController {

public:
  Position pos;
  KeyController();
  void keyscan();
  inline bool isPressed(){
    return pressed;
  }
private:
  void update();
  bool pressed;
  Position      firstpos;
  Coordinate    lastpos;
  unsigned long lastpressed;
  unsigned long lasttapped;
  unsigned long lastreleased;
};

#endif /* _KEYCONTROLLER_H_ */
