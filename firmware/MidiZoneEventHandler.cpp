#include "MidiZoneEventHandler.h"
#include "globals.h"

void MidiZoneEventHandler::taptap(Position& pos){
  if(blipbox.keys.pos.column == 0 && blipbox.keys.pos.row == 0)
    blipbox.setEditMode(true);
}

void MidiZoneEventHandler::tick(uint16_t counter){
//   if(counter % MIDI_ZONES_IN_PRESET == 0)
//     blipbox.leds.clear();
//   zones[counter % MIDI_ZONES_IN_PRESET].tick();
//   if(counter % MIDI_ZONES_IN_PRESET == MIDI_ZONES_IN_PRESET-1)
//     blipbox.leds.flip();
  blipbox.leds.clear();
  for(int i=0; i<MIDI_ZONES_IN_PRESET; ++i)
    zones[i].tick();
  blipbox.leds.flip();
}
