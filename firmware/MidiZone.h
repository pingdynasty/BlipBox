#ifndef _MIDIZONE_H_
#define _MIDIZONE_H_

#include <inttypes.h>
#include "Position.h"
#include "defs.h"
#include "MidiConstants.h"

#define BUTTON_SLIDER_ZONE_BIT       (1<<4)
#define HORIZONTAL_VERTICAL_ZONE_BIT (1<<5) // bit 5 used by sliders
#define MOMENTARY_TOGGLE_ZONE_BIT    (1<<5) // bit 5 used by buttons
#define INVERTED_ZONE_BIT            (1<<7)

#define ZONE_TYPE_MASK                0x03
#define EMPTY_ZONE_TYPE               0x00
#define MIDI_ZONE_TYPE                0x01
#define NRPN_ZONE_TYPE                0x02
#define SELECTOR_ZONE_TYPE            0x03

#define DISPLAY_TYPE_MASK             0x0c
#define NO_DISPLAY_TYPE               0x00
#define DOT_DISPLAY_TYPE              (1<<2)
#define BAR_DISPLAY_TYPE              (2<<2)
#define FILL_DISPLAY_TYPE             (3<<2)

// for buttons the on/off values are determined by _min and _max

class MidiZone {
protected:
public:
  uint8_t _type;
  uint8_t _status;
  int8_t _data1, _data2;
  int8_t _min, _max;
  int8_t _from_column, _to_column;
  int8_t _from_row, _to_row;
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
  void load(uint8_t* index);
  virtual bool doDrag();
  virtual bool doPress();
  virtual bool doRelease();
  virtual bool doTick();
  virtual void release(Position& pos);
  virtual void press(Position& pos);
  virtual void drag(Position& pos);
  virtual void sendMessage(uint8_t data1, uint8_t data2);
//   void tap(Position& pos);
  void tick();
};

#endif /* _MIDIZONE_H_ */
