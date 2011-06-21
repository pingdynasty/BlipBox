#include "MidiZoneEventHandler.h"
#include "globals.h"

void MidiZoneEventHandler::taptap(Position& pos){
  if(blipbox.keys.getColumn() == 0 && blipbox.keys.getRow() == 0)
    blipbox.setEditMode(true);
}

void MidiZoneEventHandler::tick(uint16_t counter){
  if(counter % MIDI_ZONES == 0)
    blipbox.leds.clear();
  zones[counter % MIDI_ZONES].tick();
}
