#ifndef _MIDIZONEEVENTHANDLER_H_
#define _MIDIZONEEVENTHANDLER_H_

#include <inttypes.h>
#include "EventHandler.h"
#include "MidiZone.h"
#include "Animator.h"

#define MIDI_ZONES 8

class MidiZoneEventHandler : public EventHandler, public Animator {
private:
  MidiZone zones[MIDI_ZONES];
  uint8_t preset;
public:
  void init(){}
  void setPreset(uint8_t index){
    preset = index;
  }
  uint8_t getPreset(){
    return preset;
  }
  void loadPreset(int index){
    preset = index;
    if(preset == 4){
      uint8_t data[] = {1, 33, 0x22, 0x88};
      zones[0].read(data);
    }else{
      index = index*MIDI_ZONES;
      for(int i=0; i<MIDI_ZONES; ++i)
	zones[i].load(i+index);
    }
  }
  void savePreset(int index){
    preset = index;
    index = index*MIDI_ZONES;
    for(int i=0; i<MIDI_ZONES; ++i)
      zones[i].save(i+index);
  }

  MidiZone& getZone(uint8_t zone){
    return zones[zone];
  }

  void press(Position& pos){
    for(int i=0; i<MIDI_ZONES; ++i)
      if(zones[i].check(pos))
        zones[i].press(pos);
  }

  void release(Position& pos){
    for(int i=0; i<MIDI_ZONES; ++i)
      if(zones[i].check(pos))
        zones[i].release(pos);
  }

  void drag(Position& pos){
    for(int i=0; i<MIDI_ZONES; ++i)
      if(zones[i].check(pos))
        zones[i].drag(pos);
  }

  void tap(Position& pos){
    for(int i=0; i<MIDI_ZONES; ++i)
      if(zones[i].check(pos))
        zones[i].tap(pos);
  }

  void taptap(Position& pos);

  void tick(uint16_t counter);
};

#endif /* _MIDIZONEEVENTHANDLER_H_ */
