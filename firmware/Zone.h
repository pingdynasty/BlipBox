#ifndef _ZONE_H_
#define _ZONE_H_

#include "Position.h"
#include "Event.h"
#include "Action.h"

#define ZONE_TYPE_MASK              0xf0
#define DISPLAY_TYPE_MASK           0x0f

#define NONE_DISPLAY_TYPE           0x00
#define FILL_DISPLAY_TYPE           0x01
#define LINE_DISPLAY_TYPE           0x02

#define DISABLED_ZONE_TYPE          0x00
#define HORIZONTAL_SLIDER_ZONE_TYPE 0x10
#define VERTICAL_SLIDER_ZONE_TYPE   0x30
#define MOMENTARY_BUTTON_ZONE_TYPE  0x20
#define TOGGLE_BUTTON_ZONE_TYPE     0x40

class Zone {
private:
  uint8_t type;
public:
  Action* action;
  Coordinate from;
  Coordinate to;
  Zone();
  uint8_t read(const uint8_t* data);
  uint8_t write(uint8_t* data);
  bool match(Position* pos){
    return *pos < to && from <= *pos;
  }
  void setZoneType(uint8_t type);
  uint8_t getDisplayType(){
    return type & DISPLAY_TYPE_MASK;
  }
  void setDisplayType(uint8_t value){
    type = (ZONE_TYPE_MASK & type) | (DISPLAY_TYPE_MASK & value);
  }
  uint8_t getZoneType() { return ZONE_TYPE_MASK & type; }
  virtual void handle(TouchEvent& event){}
  virtual void line(){}
  virtual void fill(){}
};

#endif /* _ZONE_H_ */
