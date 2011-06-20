#include "MidiZoneEventHandler.h"
#include "globals.h"

void MidiZoneEventHandler::taptap(Position& pos){
  if(blipbox.keys.getColumn() == 0 && blipbox.keys.getRow() == 0)
    blipbox.setEditMode(true);
}
