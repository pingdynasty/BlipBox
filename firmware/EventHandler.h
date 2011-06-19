#ifndef _EVENTHANDLER_H_
#define _EVENTHANDLER_H_

#include <stdint.h>
#include "Position.h"
#include "Animator.h"

class EventHandler {
public:
  virtual void init(){}
  virtual void press(Position& pos){}
  virtual void release(Position& pos){}
  virtual void drag(Position& pos){}
  virtual void tap(Position& pos){}
  virtual void taptap(Position& pos){}
};

class PresetChooser : public EventHandler, public Animator {
private:
  uint8_t preset;
  void printPreset();
public:
  void init();
  void tick(uint16_t counter);
  void tap(Position& pos);
  void taptap(Position& pos);
};

// class MidiZonesEventHandler : public EventHandler {
// };

class DefaultEventHandler : public EventHandler {
public:
  void press(Position& pos);
  void release(Position& pos);
  void drag(Position& pos);
  void taptap(Position& pos);
};


#endif /* _EVENTHANDLER_H_ */
