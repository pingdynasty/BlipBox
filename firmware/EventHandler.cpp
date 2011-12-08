#include "globals.h"
#include "EventHandler.h"
#include "SerialProtocol.h"

void DefaultEventHandler::handle(TouchEvent& event){
  if(event.isTouch()){
    if(event.isPress() || event.isDrag()){
      sendPositionMessage(event.getPosition()->x, event.getPosition()->y);
    }else if(event.isRelease()){
      sendReleaseMessage();
    }else if(event.isTapTap()){
      if(event.getPosition()->getColumn() == 0 && event.getPosition()->getRow() == 0)
	blipbox.setEditMode(true);
    }
  }
}

