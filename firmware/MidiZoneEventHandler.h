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
  MidiZone& getZone(uint8_t zone){
    return zones[zone];
  }
  void loadPreset(uint8_t index);
  void savePreset(uint8_t index);
  void press(Position& pos);
  void release(Position& pos);
  void drag(Position& pos);
//   void tap(Position& pos);
  void taptap(Position& pos);
  void tick(uint16_t counter);
};

#endif /* _MIDIZONEEVENTHANDLER_H_ */
