#ifndef _PRESETCHOOSER_H_
#define _PRESETCHOOSER_H_

#include <stdint.h>
#include "Position.h"
#include "Animator.h"
#include "EventHandler.h"

class PresetChooser : public EventHandler, public Animator {
private:
  uint8_t preset;
  uint8_t column, origin;
  void printPreset();
public:
  PresetChooser();
  void tick(uint16_t counter);
  void handle(TouchEvent& event);
  void press(Position& pos);
  void release();
  void drag(Position& pos);
  void taptap(Position& pos);

};

#endif /* _PRESETCHOOSER_H_ */
