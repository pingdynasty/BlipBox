#include <stdint.h>
#include "globals.h"
#include "EventHandler.h"

void DefaultEventHandler::press(Position& pos){
  blipbox.sender.position.update(pos);
  blipbox.sender.release.reset();
}

void DefaultEventHandler::release(Position& pos){
  blipbox.sender.position.reset();
  blipbox.sender.release.update();
}

void DefaultEventHandler::drag(Position& pos){
  blipbox.sender.position.update(pos);
}

void DefaultEventHandler::taptap(Position& pos){
  if(blipbox.keys.getColumn() == 0 && blipbox.keys.getRow() == 0)
    blipbox.setEditMode(true);
}
