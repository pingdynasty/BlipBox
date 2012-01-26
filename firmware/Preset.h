#ifndef _PRESET_H_
#define _PRESET_H_

#include "EventHandler.h"
#include "Animator.h"
#include "Zone.h"
#include "Event.h"

#define MAX_ZONES_IN_PRESET 8

class Preset : public EventHandler, public Animator {
private:
  Zone* zones[MAX_ZONES_IN_PRESET];
public:
  Preset();
  void handle(TouchEvent& event);
  void handle(MidiEvent& event);
/*   void configure(uint8_t* data); */
  void tick(uint16_t counter);
  Zone* getZone(uint8_t index){
    if(index < MAX_ZONES_IN_PRESET)
      return zones[index];
    return NULL;
  }
  void setZone(uint8_t index, Zone* zone){
    delete zones[index];
    zones[index] = zone;
  }
  uint8_t getNumberOfZones(){
    int i=0;
    for(;i<MAX_ZONES_IN_PRESET && zones[i] != NULL; ++i);
    return i;
  }
  void load(uint8_t index);
  void save(uint8_t index);
  uint8_t readZone(const uint8_t* data, uint8_t index);
  uint8_t writeZone(uint8_t* data, uint8_t index);
};

#endif /* _PRESET_H_ */
