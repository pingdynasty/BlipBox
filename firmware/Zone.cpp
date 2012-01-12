#include "Zone.h"
#include "MidiConstants.h"
#include "globals.h"
#include <stddef.h>

#define SENSOR_RANGE 1023.0
#define COLUMN_RANGE   10.0
#define ROW_RANGE       8.0

// Zone::Zone() : action(NULL) {}
Zone::Zone() {}

class MomentaryButtonZone : public Zone {
public:
  void handle(TouchEvent& event){
    if(event.isPress())
      action->on(MAX_DATA_VALUE);
    else if(event.isRelease())
      action->on(MIN_DATA_VALUE);
  }
  void fill(){
    if(action->getValue() != MIN_DATA_VALUE){
      uint8_t intensity = action->getValue() * blipbox.config.brightness;
      blipbox.display.fill(from.getColumn(), from.getRow(), 
			   to.getColumn(), to.getRow(), intensity);
    }
  }
  void line(){
    // a button in line mode is displayed as a diagonal line
    uint8_t intensity = action->getValue() * blipbox.config.brightness;
    if(action->getValue() == MIN_DATA_VALUE){
      blipbox.display.line(from.getColumn(), from.getRow(),
			   to.getColumn()-1, to.getRow()-1, blipbox.config.brightness / 2);
    }else{
      blipbox.display.line(from.getColumn(), to.getRow()-1,
			   to.getColumn()-1, from.getRow(), intensity);
    }
  }
};

class ToggleButtonZone : public MomentaryButtonZone {
public:
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
  float scaleToFloat(Position* pos){
    return ((pos->x/SENSOR_RANGE-(from.getColumn()/COLUMN_RANGE))/
	    ((to.getColumn()-from.getColumn())/COLUMN_RANGE));
  }
  uint8_t getColumn(){
    uint8_t col = (uint8_t)(action->getValue()*(to.getColumn()-from.getColumn()))+from.getColumn();
    return min(max(col, from.getColumn()), to.getColumn()-1);
  }
  void fill(){
    uint8_t col = getColumn() + 1;
    // it's a bit weird that the fill range does excludes end point,
    // while line is inclusive
    uint8_t intensity = action->getValue() * blipbox.config.brightness;
    blipbox.display.fill(from.getColumn(), from.getRow(), col, to.getRow(), intensity);
  }
  void line(){
    uint8_t col = getColumn();
    uint8_t intensity = action->getValue() * blipbox.config.brightness;
    blipbox.display.line(col, from.getRow(), col, to.getRow()-1, intensity);
  }
};

class VerticalSliderZone : public SliderZone {
public:
  float scaleToFloat(Position* pos){
    return ((pos->y/SENSOR_RANGE-(from.getRow()/ROW_RANGE))/
	    ((to.getRow()-from.getRow())/ROW_RANGE));
  }
  uint8_t getRow(){
    uint8_t row = (uint8_t)(action->getValue()*(to.getRow()-from.getRow()))+from.getRow();
    return min(max(row, from.getRow()), to.getRow()-1);
  }
  void fill(){
    uint8_t row = getRow() + 1;
    uint8_t intensity = action->getValue() * blipbox.config.brightness;
    blipbox.display.fill(from.getColumn(), from.getRow(), to.getColumn(), row, intensity);
  }
  void line(){
    uint8_t row = getRow();
    uint8_t intensity = action->getValue() * blipbox.config.brightness;
    blipbox.display.line(from.getColumn(), row, to.getColumn()-1, row, intensity);
  }
};

uint8_t Zone::write(uint8_t* data){
  data[0] = type;
  data[1] = from.getValue();
  data[2] = to.getValue();
  if(action == NULL)
    return 3;
  return 3+action->write(data);
}

// see http://en.wikipedia.org/wiki/Placement_syntax
void * operator new (size_t, void * p); // defined in operators.cpp

void Zone::setZoneType(uint8_t value){
  type = (ZONE_TYPE_MASK & value) | (DISPLAY_TYPE_MASK & type);
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
  setZoneType(data[0]);
//   setDisplayType(data[0]);
  from.setValue(data[1]);
  to.setValue(data[2]);
  delete action;
  action = Action::createAction(data[3]);
  if(action == NULL)
    return 3;
  return 3+action->read(data);
}
