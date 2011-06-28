#ifndef _MIDIZONEEVENTHANDLER_H_
#define _MIDIZONEEVENTHANDLER_H_

#include <inttypes.h>
#include "EventHandler.h"
#include "MidiZone.h"
#include "Animator.h"

class MidiZoneEventHandler : public EventHandler, public Animator {
private:
  MidiZone zones[MIDI_ZONES_IN_PRESET];
public:
  uint8_t preset;

  void loadPreset(int index){
    preset = index;
    index = index*MIDI_ZONES_IN_PRESET;
    for(int i=0; i<MIDI_ZONES_IN_PRESET; ++i)
      zones[i].load(i+index);
  }

  void savePreset(int index){
    preset = index;
    index = index*MIDI_ZONES_IN_PRESET;
    for(int i=0; i<MIDI_ZONES_IN_PRESET; ++i)
      zones[i].save(i+index);
  }

  MidiZone& getZone(uint8_t zone){
    return zones[zone];
  }

  void press(Position& pos){
    for(int i=0; i<MIDI_ZONES_IN_PRESET; ++i)
      if(zones[i].check(pos))
        zones[i].press(pos);
  }

  void release(Position& pos){
    for(int i=0; i<MIDI_ZONES_IN_PRESET; ++i)
      if(zones[i].check(pos))
        zones[i].release(pos);
  }

  void drag(Position& pos){
    for(int i=0; i<MIDI_ZONES_IN_PRESET; ++i)
      if(zones[i].check(pos))
        zones[i].drag(pos);
  }

//   void tap(Position& pos){
//     for(int i=0; i<MIDI_ZONES_IN_PRESET; ++i)
//       if(zones[i].check(pos))
//         zones[i].tap(pos);
//   }

  void taptap(Position& pos);

  void tick(uint16_t counter);
};

#endif /* _MIDIZONEEVENTHANDLER_H_ */
