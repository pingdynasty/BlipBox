#ifndef _MIDIZONE_H_
#define _MIDIZONE_H_

#include <inttypes.h>
#include "Position.h"
#include "defs.h"

class MidiZone {
private:
  uint8_t _type;
  uint8_t _status;
  int8_t _data1, _data2;
  int8_t _from_x, _to_x;
  int8_t _from_y, _to_y;
  void channelMessage(uint8_t data1, uint8_t data2);
public:
  void read(const uint8_t* data);
  void write(uint8_t* data);
  void save(uint8_t index);
  void load(uint8_t index);
  inline bool check(const Position& pos){
    return _from_x <= pos.column && pos.column < _to_x &&
           _from_y <= pos.row && pos.row < _to_y;
  }
//   bool check(uint8_t x, uint8_t y){
//     return _from_x<=x && x<_to_x && _from_y<=y && y<_to_y;
//   }
  void release(Position& pos);
  uint8_t getx();
  uint8_t gety();
  void press(Position& pos);
  void drag(Position& pos);
  void tap(Position& pos);
  void tick();
};

#endif /* _MIDIZONE_H_ */
