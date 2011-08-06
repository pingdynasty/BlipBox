#ifndef _MIDIZONE_H_
#define _MIDIZONE_H_

#include <inttypes.h>
#include "Position.h"
#include "defs.h"

class MidiZone {
protected:
public:
  uint8_t _type;
  uint8_t _status;
  int8_t _data1, _data2;
  uint8_t _min, _max;
  int8_t _from_column, _to_column;
  int8_t _from_row, _to_row;
  void sendMessage(uint8_t data1, uint8_t data2);
  float scale(Position& pos);
  uint8_t scale7(Position& pos);
  uint16_t scale14(Position& pos);
  uint8_t getx();
  uint8_t gety();
public:
  inline bool check(const Position& pos){
    return _from_column <= pos.column && pos.column < _to_column &&
           _from_row <= pos.row && pos.row < _to_row;
  }
  void read(const uint8_t* data);
  void write(uint8_t* data);
//   void save(uint8_t index);
  void load(uint8_t index);
  virtual bool doDrag();
  virtual bool doPress();
  virtual bool doRelease();
  virtual void release(Position& pos);
  virtual void press(Position& pos);
  virtual void drag(Position& pos);
//   void tap(Position& pos);
  void tick();
};

#endif /* _MIDIZONE_H_ */
