#ifndef _EVENTHANDLER_H_
#define _EVENTHANDLER_H_

#include <stdint.h>
#include "Position.h"

class EventHandler {
public:
  virtual void init(){}
  virtual void press(Position& pos){}
  virtual void release(Position& pos){}
  virtual void drag(Position& pos){}
  virtual void tap(Position& pos){}
  virtual void taptap(Position& pos){}
};

class DefaultEventHandler : public EventHandler {
public:
  void press(Position& pos);
  void release(Position& pos);
  void drag(Position& pos);
  void taptap(Position& pos);
};


#endif /* _EVENTHANDLER_H_ */
