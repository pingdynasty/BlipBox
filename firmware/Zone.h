#ifndef _ZONE_H_
#define _ZONE_H_

#include "Position.h"
#include "Event.h"
#include "Action.h"

#define ZONE_TYPE_MASK              0xf0
#define DISPLAY_TYPE_MASK           0x0f

// #define NONE_DISPLAY_TYPE           0x00
// #define FILL_DISPLAY_TYPE           0x01
// #define LINE_DISPLAY_TYPE           0x02

// #define DISABLED_ZONE_TYPE          0x00
// #define HORIZONTAL_SLIDER_ZONE_TYPE 0x10
// #define VERTICAL_SLIDER_ZONE_TYPE   0x30
// #define MOMENTARY_BUTTON_ZONE_TYPE  0x20
// #define TOGGLE_BUTTON_ZONE_TYPE     0x40

enum ZoneType {
   DISABLED_ZONE_TYPE         =  0x00,
   HORIZONTAL_SLIDER_ZONE_TYPE=  0x10,
   VERTICAL_SLIDER_ZONE_TYPE  =  0x30,
   MOMENTARY_BUTTON_ZONE_TYPE =  0x20,
   TOGGLE_BUTTON_ZONE_TYPE    =  0x40
};

enum DisplayType {
  NONE_DISPLAY_TYPE          =  0x00,
  FILL_DISPLAY_TYPE          =  0x01,
  LINE_DISPLAY_TYPE          =  0x02,
  GRADED_FILL_DISPLAY_TYPE   =  0x03,
  GRADED_LINE_DISPLAY_TYPE   =  0x04
 };

class Zone {
private:
  uint8_t type;
public:
  Action* action;
  Coordinate from;
  Coordinate to;
  Zone();
  ~Zone();
  uint8_t read(const uint8_t* data);
  uint8_t write(uint8_t* data);
  bool match(Position* pos){
    return *pos < to && from <= *pos;
  }
  DisplayType getDisplayType(){
    return (DisplayType)(type & DISPLAY_TYPE_MASK);
  }
  void setDisplayType(DisplayType value){
    type = (DisplayType)((ZONE_TYPE_MASK & type) | (DISPLAY_TYPE_MASK & value));
  }
  ZoneType getZoneType() { return (ZoneType)(ZONE_TYPE_MASK & type); }
//   void setZoneType(ZoneType type);
  virtual void handle(TouchEvent& event){}
  virtual void line(uint8_t intensity){}
  virtual void fill(uint8_t intensity){}
  static Zone* createZone(uint8_t type);
};

#endif /* _ZONE_H_ */
