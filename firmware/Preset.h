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
  uint8_t size;
public:
  Preset();
  void handle(TouchEvent& event);
  void handle(MidiEvent& event);
/*   void configure(uint8_t* data); */
  void tick(uint16_t counter);
  void clear(){
    while(size)
      delete zones[--size];
  }
  Zone* getZone(uint8_t index){
    if(index >= size)
      return NULL;
    return zones[index];
  }
  Zone* removeZone(uint8_t index){
    if(index >= size)
      return NULL;
    Zone* zone = zones[index];    
    --size;
    for(; index<size; index++)
      zones[index] = zones[index+1];
    zones[size] = NULL;
    return zone;
  }
  void addZone(Zone* zone){
    if(size < MAX_ZONES_IN_PRESET)
      zones[size++] = zone;
  }
/*   void setZone(uint8_t index, Zone* zone){ */
/*     delete zones[index]; */
/*     zones[index] = zone; */
/*   } */
  uint8_t getNumberOfZones(){
    return size;
  }
/*   uint8_t getTotalSize(){ */
/*     int length = 0; */
/*     for(uint8_t i=0; i<size; ++i) */
/*       length += zone[i].write(); */
/*     return length; */
/*   } */
  void load(uint8_t index);
  void save(uint8_t index);
  uint8_t readZone(const uint8_t* data, uint8_t index);
  uint8_t writeZone(uint8_t* data, uint8_t index);
};

#endif /* _PRESET_H_ */
