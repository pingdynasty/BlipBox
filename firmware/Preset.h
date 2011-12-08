#ifndef _PRESET_H_
#define _PRESET_H_

#include "EventHandler.h"
#include "Animator.h"
#include "Zone.h"
#include "Event.h"

#define MAX_ZONES_IN_PRESET 8

class Preset : public EventHandler, public Animator {
private:
  Zone zones[MAX_ZONES_IN_PRESET];

public:
  void handle(TouchEvent& event);
  void handle(MidiEvent& event);
  void load(uint8_t index);
  void save(uint8_t index);
/*   void configure(uint8_t* data); */
  void tick(uint16_t counter);
  Zone* getZone(uint8_t index){
    return &zones[index];
  }
};

#endif /* _PRESET_H_ */
