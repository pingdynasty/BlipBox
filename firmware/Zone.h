#ifndef _ZONE_H_
#define _ZONE_H_

#include "Position.h"
#include "Event.h"
#include "Action.h"

#define ACTION_TYPE_MASK 0x0f
#define ZONE_TYPE_MASK   0xf0
#define MIDI_ACTION_TYPE 0x01

#define HORIZONTAL_SLIDER_ZONE_TYPE 0x10
#define VERTICAL_SLIDER_ZONE_TYPE   0x30
#define MOMENTARY_BUTTON_ZONE_TYPE  0x20
#define TOGGLE_BUTTON_ZONE_TYPE     0x40

class Zone {
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
  void setType(uint8_t type);
  virtual uint8_t getType() { return 0; }
  virtual void handle(TouchEvent& event){}
  virtual void draw(){}
};

#endif /* _ZONE_H_ */
