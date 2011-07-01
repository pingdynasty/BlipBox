#include "MidiZoneEventHandler.h"
#include "globals.h"

void MidiZoneEventHandler::taptap(Position& pos){
  if(blipbox.keys.pos.column == 0 && blipbox.keys.pos.row == 0)
    blipbox.setEditMode(true);
}

void MidiZoneEventHandler::tick(uint16_t counter){
  if(counter % 0x100 == 0){
    blipbox.leds.clear();
    for(int i=0; i<MIDI_ZONES_IN_PRESET; ++i)
      zones[i].tick();
  }
}
