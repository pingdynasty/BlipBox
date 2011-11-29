#ifndef _EVENT_H_
#define _EVENT_H_

#include "Position.h"

#define TOUCH_EVENT_MASK    0x01<<7
#define RELEASE_EVENT_TYPE -0x01
#define PRESS_EVENT_TYPE   -0x03
#define TAP_EVENT_TYPE     -0x05
#define TAPTAP_EVENT_TYPE  -0x07
#define DRAG_EVENT_TYPE    -0x09

class Event {
public:
  int8_t type;
  Position* pos;
  Event(Position* apos)
    : pos(apos) {}
/*   Event(uint8_t atype) */
/*     : type(atype) {} */

  Position* getPosition();

  bool isTouch(){
    return type & TOUCH_EVENT_MASK;
  }

  bool isTap(){
    return type == TAP_EVENT_TYPE;
  }

  bool isTapTap(){
    return type == TAPTAP_EVENT_TYPE;
  }

  bool isDrag(){
    return type == DRAG_EVENT_TYPE;
  }

  bool isRelease(){
    return type == RELEASE_EVENT_TYPE;
  }
};


/* enum EventType { */
/*   RELEASE, TAP, TAPTAP, DRAG */
/* }; */

/* class Event { */
/*   Position pos; */
/* public: */
/*   Position getPosition(){ */
/*     return pos; */
/*   } */
/*   EventType getType(){ */
/*     return RELEASE; */
/*   } */
/* }; */

#endif /* _EVENT_H_ */
