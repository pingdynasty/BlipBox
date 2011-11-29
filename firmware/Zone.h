#ifndef _ZONE_H_
#define _ZONE_H_

#include "Position.h"
#include "Event.h"
#include "Action.h"

class Zone {
public:
  Action* action;
  Coordinate from;
  Coordinate to;
  void read(const uint8_t* data);
  void write(uint8_t* data);
  bool match(Position* pos){
    return *pos < to && from <= *pos;
  }
  virtual void handle(Event& event){}
  virtual void draw(){}
};

#endif /* _ZONE_H_ */
