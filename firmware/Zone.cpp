#include "Zone.h"
#include "MidiConstants.h"
#include "globals.h"
#include <stddef.h>

#define SENSOR_RANGE 1023.0
#define COLUMN_RANGE   10.0
#define ROW_RANGE       8.0

Zone::Zone() : action(NULL) {}

class MomentaryButtonZone : public Zone {
public:
  uint8_t getType(){
    return MOMENTARY_BUTTON_ZONE_TYPE;
  }
  void handle(TouchEvent& event){
    if(event.isPress())
      action->on(MAX_DATA_VALUE);
    else if(event.isRelease())
      action->on(MIN_DATA_VALUE);
  }
  void draw(){
    if(action->getValue() != MIN_DATA_VALUE){
      blipbox.display.fill(from.getColumn(), from.getRow(), 
			   to.getColumn(), to.getRow(), blipbox.config.brightness);
    }
  }
};

class ToggleButtonZone : public MomentaryButtonZone {
public:
  uint8_t getType(){
    return TOGGLE_BUTTON_ZONE_TYPE;
  }
  virtual void handle(TouchEvent& event){
    if(event.isPress()){
      if(action->getValue() != MIN_DATA_VALUE)
	action->on(MIN_DATA_VALUE);
      else
	action->on(MAX_DATA_VALUE);
    }
  }
};

class SliderZone : public Zone {
public:
  void handle(TouchEvent& event){
    if(event.isDrag())
      action->on(scaleToFloat(event.getPosition()));
    else if(event.isRelease())
      action->off();
  }
  virtual float scaleToFloat(Position* pos){
    return 0;
  }
};

class HorizontalSliderZone : public SliderZone {
public:
  uint8_t getType(){
    return HORIZONTAL_SLIDER_ZONE_TYPE;
  }
  float scaleToFloat(Position* pos){
    return ((pos->x/SENSOR_RANGE-(from.getColumn()/COLUMN_RANGE))/
	    ((to.getColumn()-from.getColumn())/COLUMN_RANGE));
  }
  void draw(){
    uint8_t col = (uint8_t)(action->getValue()*(to.getColumn()-from.getColumn()))+from.getColumn();
    blipbox.display.line(col, from.getRow(), col, to.getRow()-1, blipbox.config.brightness);
  }
};

class VerticalSliderZone : public SliderZone {
public:
  uint8_t getType(){
    return VERTICAL_SLIDER_ZONE_TYPE;
  }
  float scaleToFloat(Position* pos){
    return ((pos->y/SENSOR_RANGE-(from.getRow()/ROW_RANGE))/
	    ((to.getRow()-from.getRow())/ROW_RANGE));
  }
  void draw(){
    uint8_t row = (uint8_t)(action->getValue()*(to.getRow()-from.getRow()))+from.getRow();
    blipbox.display.line(from.getColumn(), row, to.getColumn()-1, row, blipbox.config.brightness);
  }
};

uint8_t Zone::write(uint8_t* data){
  data[0] = getType();
  data[1] = from.getValue();
  data[2] = to.getValue();
  if(action == NULL)
    return 3;
  return 3+action->write(data);
}

// see http://en.wikipedia.org/wiki/Placement_syntax
void * operator new (size_t, void * p); // defined in operators.cpp

void Zone::setType(uint8_t type){
  switch(type & ZONE_TYPE_MASK){
  case HORIZONTAL_SLIDER_ZONE_TYPE:
    new(this)HorizontalSliderZone();
    break;
  case VERTICAL_SLIDER_ZONE_TYPE:
    new(this)VerticalSliderZone();
    break;
  case MOMENTARY_BUTTON_ZONE_TYPE:
    new(this)MomentaryButtonZone();
    break;
  case TOGGLE_BUTTON_ZONE_TYPE:
    new(this)ToggleButtonZone();
    break;
  default:
    new(this)Zone();
  }
}

uint8_t Zone::read(const uint8_t* data){
  setType(data[0]);
  from.setValue(data[1]);
  to.setValue(data[2]);
  delete action;
  action = Action::createAction(data[3]);
  if(action == NULL)
    return 3;
  return 3+action->read(data);
}
