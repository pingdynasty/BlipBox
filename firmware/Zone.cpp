#include "Zone.h"
#include "MidiConstants.h"
#include "globals.h"
#include <stddef.h>

#define SENSOR_RANGE 1023.0
#define COLUMN_RANGE   10.0
#define ROW_RANGE       8.0

Zone::Zone() : type(DISABLED_ZONE_TYPE|LINE_DISPLAY_TYPE), 
	       action(NULL), from(0,0), to(10, 8) {}

Zone::~Zone() {
  delete action;
}

class MomentaryButtonZone : public Zone {
public:
  void handle(TouchEvent& event){
    if(event.isPress())
      action->on(MAX_DATA_VALUE);
    else if(event.isRelease())
      action->on(MIN_DATA_VALUE);
  }
  void fill(uint8_t intensity){
    if(action->getValue() != MIN_DATA_VALUE){
      blipbox.display.fill(from.getColumn(), from.getRow(), 
			   to.getColumn(), to.getRow(), intensity);
    }
  }
  void line(uint8_t intensity){
    // a button in line mode is displayed as a diagonal line
    if(action->getValue() == MIN_DATA_VALUE){
      blipbox.display.line(from.getColumn(), from.getRow(),
			   to.getColumn()-1, to.getRow()-1, intensity / 2);
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
  void fill(uint8_t intensity){
    uint8_t col = getColumn();
    // it's a bit weird that the fill range does excludes end point,
    // while line is inclusive
    if(isInverted())
      blipbox.display.fill(col, from.getRow(), to.getColumn(), to.getRow(), intensity);
    else
      blipbox.display.fill(from.getColumn(), from.getRow(), col+1, to.getRow(), intensity);
  }
  void line(uint8_t intensity){
    uint8_t col = getColumn();
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
  void fill(uint8_t intensity){
    uint8_t row = getRow();
    if(isInverted())
      blipbox.display.fill(from.getColumn(), row, to.getColumn(), to.getRow(), intensity);
    else
      blipbox.display.fill(from.getColumn(), from.getRow(), to.getColumn(), row+1, intensity);
  }
  void line(uint8_t intensity){
    uint8_t row = getRow();
    blipbox.display.line(from.getColumn(), row, to.getColumn()-1, row, intensity);
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

uint8_t Zone::read(const uint8_t* data){
//   setZoneType((ZoneType)(ZONE_TYPE_MASK & data[0]));
//   setDisplayType((DisplayType)(DISPLAY_TYPE_MASK & data[0]));
  type = data[0];
  from.setValue(data[1]);
  to.setValue(data[2]);
  delete action;
  action = Action::createAction(data[3]);
  if(action == NULL)
    return 3;
  return 3+action->read(data);
}

// static method
Zone* Zone::createZone(uint8_t type){
  Zone* zone = NULL;
  switch(type & ZONE_TYPE_MASK){
  case HORIZONTAL_SLIDER_ZONE_TYPE:
    zone = new HorizontalSliderZone();
    break;
  case VERTICAL_SLIDER_ZONE_TYPE:
    zone = new VerticalSliderZone();
    break;
  case MOMENTARY_BUTTON_ZONE_TYPE:
    zone = new MomentaryButtonZone();
    break;
  case TOGGLE_BUTTON_ZONE_TYPE:
    zone = new ToggleButtonZone();
    break;
  default:
    zone = new Zone();
  }
  zone->type = type;
  return zone;
}
