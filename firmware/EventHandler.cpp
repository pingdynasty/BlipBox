#include "globals.h"
#include "EventHandler.h"
#include "SerialProtocol.h"

void DefaultEventHandler::handle(TouchEvent& event){
  switch(event.getType()){
  case RELEASE_EVENT_TYPE:
    sendReleaseMessage();
    break;
  case TAPTAP_EVENT_TYPE:
    if(event.getPosition()->getColumn() == 0 && event.getPosition()->getRow() == 0)
      blipbox.setMode(EDIT_MODE);
    break;
  case PRESS_EVENT_TYPE:
  case DRAG_EVENT_TYPE:
    sendPositionMessage(event.getPosition()->x, event.getPosition()->y);
    break;
  }
}

