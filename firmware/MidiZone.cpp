#include "MidiZone.h"
#include "globals.h"

void MidiZone::tap(Position& pos){
  switch(_type){
  case NOTE_TOGGLE_BUTTON:
    _status ^= 0x10;
    // deliberate fall-through
  case CC_TOGGLE_BUTTON:
    _data2 = _data2 ? 0 : 127;
    channelMessage(_status, _data1, _data2);
    break;
  case PRESET_BUTTON:
    if(blipbox.midizones.getPreset() != _data1)
      blipbox.midizones.loadPreset(_data1);
    break;
  }
}

void MidiZone::tick(){
  switch(_type){
  case HORIZONTAL_CC:
  case VERTICAL_NOTE:
    for(int i=_from_y; i<_to_y; ++i)
      blipbox.leds.setLed(_data2, i, blipbox.config.brightness);
    break;
  case VERTICAL_CC:
  case HORIZONTAL_NOTE:
    for(int i=_from_x; i<_to_x; ++i)
      blipbox.leds.setLed(i, _data2, blipbox.config.brightness);
    break;
  case NOTE_TOGGLE_BUTTON:
  case CC_TOGGLE_BUTTON:
  case NOTE_PUSH_BUTTON:
  case CC_PUSH_BUTTON:
    if(_data2)
      for(int x=_from_x; x<_to_x; ++x)
	for(int y=_from_y; y<_to_y; ++y)
	  blipbox.leds.setLed(x, y, blipbox.config.brightness);
    break;
  case PRESET_BUTTON:
    if(blipbox.midizones.getPreset() == _data1)
      for(int x=_from_x; x<_to_x; ++x)
	for(int y=_from_y; y<_to_y; ++y)
	  blipbox.leds.setLed(x, y, blipbox.config.brightness);
    break;
  }
}
