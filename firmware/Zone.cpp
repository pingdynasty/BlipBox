#include "Zone.h"
#include "MidiConstants.h"
#include "globals.h"
#include <stddef.h>

#define ACTION_TYPE_MASK 0x0f
#define ZONE_TYPE_MASK   0xf0
#define MIDI_ACTION_TYPE 0x01

#define HORIZONTAL_SLIDER_ZONE_TYPE 0x01
#define VERTICAL_SLIDER_ZONE_TYPE   0x03
#define MOMENTARY_BUTTON_ZONE_TYPE  0x02
#define TOGGLE_BUTTON_ZONE_TYPE     0x04

// see http://en.wikipedia.org/wiki/Placement_syntax
void * operator new (size_t, void * p); // defined in operators.cpp

class MomentaryButtonZone : public Zone {
  // momentary button
public:
  void handle(Event& event){
    if(event.isRelease())
      action->setValue(MIN_DATA_VALUE);
    else
      action->setValue(MAX_DATA_VALUE);
  }
  void draw(){
    if(action->getValue() != MIN_DATA_VALUE){
      blipbox.display.fill(from.getColumn(), from.getRow(), 
			   to.getColumn(), to.getRow(), blipbox.config.brightness);
    }
  }
};

class ToggleButtonZone : public MomentaryButtonZone {
  // toggle button
public:
  virtual void handle(Event& event){
    if(action->getValue() != MIN_DATA_VALUE)
      action->setValue(MIN_DATA_VALUE);
    else
      action->setValue(MAX_DATA_VALUE);
  }
};

class SliderZone : public Zone {
public:
  virtual void handle(Event& event){
    if(event.isDrag())
      action->setValue(scaleToFloat(event.getPosition()));
    else if(event.isRelease())
      action->setValue(MIN_DATA_VALUE);
  }
  virtual float scaleToFloat(Position* pos){
    return 0;
  }
};

class HorizontalSliderZone : public SliderZone {
public:
  float scaleToFloat(Position* pos){
    return ((pos->x-(from.getColumn()*MAX_DATA_VALUE/10.0))/
	    ((to.getColumn()-from.getColumn())*MAX_DATA_VALUE/10.0));
  }
  void draw(){
    uint8_t col = (uint8_t)(action->getValue()/MAX_DATA_VALUE*(to.getColumn()-from.getColumn()))+from.getColumn();
    blipbox.display.line(col, from.getRow(), col, to.getRow()-1, blipbox.config.brightness);
  }
};

class VerticalSliderZone : public SliderZone {
public:
  float scaleToFloat(Position* pos){
    return ((pos->y-(from.getColumn()*MAX_DATA_VALUE/8.0))/
	    ((to.getColumn()-from.getColumn())*MAX_DATA_VALUE/8.0));
  }
  void draw(){
    uint8_t row = (uint8_t)(action->getValue()/MAX_DATA_VALUE*(to.getRow()-from.getRow()))+from.getRow();
    blipbox.display.line(from.getColumn(), row, to.getColumn()-1, row, blipbox.config.brightness);
  }
};

void Zone::write(uint8_t* data){
//   data[0] = _type;
//   data[1] = _data1;
//   data[2] = _status;
//   data[3] = _min;
//   data[4] = _max;
  data[5] = from.getValue();
  data[6] = to.getValue();
}

void Zone::read(const uint8_t* data){
  uint8_t type   = data[0];
  uint8_t data1  = data[1];
  uint8_t status = data[2];
  uint8_t min    = data[3];
  uint8_t max    = data[4];
  uint8_t from_column = data[5] >> 4;
  uint8_t from_row    = data[5] & 0x0f;
  uint8_t to_column   = data[6] >> 4;
  uint8_t to_row      = data[6] & 0x0f;

  int8_t data2  = 0;
  /*     setControlValue(_data2, -1); */

  if(type & ACTION_TYPE_MASK == MIDI_ACTION_TYPE){
    switch(status & MIDI_STATUS_MASK){
    case MIDI_AFTERTOUCH:
    case MIDI_CONTROL_CHANGE:
      action = new MidiControllerAction(status, min, max, data1, data2);
      break;
    case MIDI_NOTE_ON:
    case MIDI_NOTE_OFF:
//       action = new MidiNoteAction(status, data1);
      break;
    case MIDI_PROGRAM_CHANGE:
      break;
    case MIDI_CHANNEL_PRESSURE:
      break;
    case MIDI_PITCH_BEND:
//       action = new MidiPitchBendAction(status);
      break;
    case MIDI_SYSTEM_MESSAGE:
      break;
    }
  }

  switch(type & ZONE_TYPE_MASK){
  case HORIZONTAL_SLIDER_ZONE_TYPE:
    new(this)HorizontalSliderZone();
    break;
  case VERTICAL_SLIDER_ZONE_TYPE:
    new(this)VerticalSliderZone();
    break;
  case MOMENTARY_BUTTON_ZONE_TYPE:
    new(this)ToggleButtonZone();
    break;
  case TOGGLE_BUTTON_ZONE_TYPE:
    new(this)ToggleButtonZone();
    break;
  default:
    new(this)Zone();
  }
}
