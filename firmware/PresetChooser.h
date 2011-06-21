#ifndef _PRESETCHOOSER_H_
#define _PRESETCHOOSER_H_

#include <stdint.h>
#include "Position.h"
#include "Animator.h"
#include "EventHandler.h"

class PresetChooser : public EventHandler, public Animator {
private:
  uint8_t preset;
  void printPreset();
public:
  void init();
  void tick(uint16_t counter);
  void press(Position& pos);
  void release(Position& pos);
  void drag(Position& pos);
/*   void tap(Position& pos); */
  void taptap(Position& pos);
};

#endif /* _PRESETCHOOSER_H_ */
