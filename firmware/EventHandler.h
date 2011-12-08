#ifndef _EVENTHANDLER_H_
#define _EVENTHANDLER_H_

#include <stdint.h>
#include "Event.h"
#include "Position.h"

class EventHandler {
public:
  virtual void handle(TouchEvent& event){}
  virtual void handle(MidiEvent& event){}
};

class DefaultEventHandler : public EventHandler {
public:
  void handle(TouchEvent& event);
//   void handle(MidiEvent& event);
};


#endif /* _EVENTHANDLER_H_ */
