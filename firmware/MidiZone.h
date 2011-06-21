#ifndef _MIDIZONE_H_
#define _MIDIZONE_H_

#include <wiring.h>
#include <avr/eeprom.h>
#include <inttypes.h>

#include "Position.h"

// ATMega168 has 512 bytes EEPROM
#define MIDI_PRESET_OFFSET 32 // arbitrary 32 bytes for configuration
#define MIDI_ZONE_PRESET_SIZE 4

enum MidiZoneType {
  HORIZONTAL_CC, VERTICAL_CC, 
  HORIZONTAL_NOTE, VERTICAL_NOTE,
  NOTE_PUSH_BUTTON, NOTE_TOGGLE_BUTTON, 
  CC_PUSH_BUTTON, CC_TOGGLE_BUTTON, PRESET_BUTTON
};

class MidiZone {
private:
  MidiZoneType _type;
  uint8_t _status;
  uint8_t _data1, _data2;
  uint8_t _from_x, _to_x;
  uint8_t _from_y, _to_y;

  inline void channelMessage(uint8_t status, uint8_t data1, uint8_t data2){
    serialWrite(status);
    serialWrite(data1 & 0x7f);
    serialWrite(data2 & 0x7f);
  }

public:
  /* Zone preset format:
     byte 0: ttttcccc 4 bits type, 4 bits channel
     byte 1: dddddddd 8 bits data1
     byte 2: xxxxyyyy zone start coordinate, 4 bits x, 4 bits y
     byte 3: xxxxyyyy zone end coordinate, 4 bits x, 4 bits y
   */

  void read(const uint8_t* data){
    _type =   (MidiZoneType)(data[0] >> 4);
    _status = data[0] & 0x0f; // set channel bits
    _data1 =  data[1];
    _from_x = data[2] >> 4;
    _from_y = data[2] & 0x0f;
    _to_x =   data[3] >> 4;
    _to_y =   data[3] & 0x0f;
    _data2 = 0;
    switch(_type){
    case HORIZONTAL_CC:
    case VERTICAL_CC:
    case CC_PUSH_BUTTON:
    case CC_TOGGLE_BUTTON:
      _status |= 0xb0;
      break;
    case HORIZONTAL_NOTE:
    case VERTICAL_NOTE:
    case NOTE_PUSH_BUTTON:
    case NOTE_TOGGLE_BUTTON:
      _status |= 0x80;
      break;
    }
  }

  void write(uint8_t* data){
    data[0] = (_type << 4) | (_status & 0x0f);
    data[1] = _data1;
    data[2] = (_from_x << 4) | _from_y;
    data[3] = (_to_x << 4) | _to_y;
  }

  void save(uint8_t index){
    index = MIDI_PRESET_OFFSET+index*MIDI_ZONE_PRESET_SIZE;
    uint8_t buf[MIDI_ZONE_PRESET_SIZE];
    write(buf);
    eeprom_write_block(buf, (uint8_t*)index, sizeof(buf));
  }

  void load(uint8_t index){
    index = MIDI_PRESET_OFFSET+index*MIDI_ZONE_PRESET_SIZE;
    uint8_t buf[MIDI_ZONE_PRESET_SIZE];
    eeprom_read_block(buf, (uint8_t*)index, sizeof(buf));
    read(buf);
  }

  bool check(const Position& pos){
    return _from_x <= pos.column && pos.column < _to_x &&
           _from_y <= pos.row && pos.row < _to_y;
  }

/*   bool check(uint16_t x, uint16_t y){ */
/*     return _from_x<=x && x<_to_x && _from_y<=y && y<_to_y; */
/*   } */

  void release(Position& pos){
    switch(_type){
    case HORIZONTAL_NOTE:
    case VERTICAL_NOTE:
      _status ^= 0x10;
      _data2 = 0;
      channelMessage(_status, _data1, _data2);
      break;
    case NOTE_PUSH_BUTTON:
      _status ^= 0x10;
      // deliberate fall-through
    case CC_PUSH_BUTTON:
      _data2 = 0;
      channelMessage(_status, _data1, _data2);
      break;
    }
  }

  void press(Position& pos){
    switch(_type){
    case HORIZONTAL_NOTE:
      _status ^= 0x10;
      _data1 = 127*(pos.x-_from_x)/(_to_x-_from_x);
      _data2 = 127*(pos.y-_from_y)/(_to_y-_from_y);
      channelMessage(_status, _data1, _data2);
      break;
    case VERTICAL_NOTE:
      _status ^= 0x10;
      _data1 = 127*(pos.y-_from_y)/(_to_y-_from_y);
      _data2 = 127*(pos.x-_from_x)/(_to_x-_from_x);
      channelMessage(_status, _data1, _data2);
      break;
    case NOTE_PUSH_BUTTON:
      _status ^= 0x10;
      // deliberate fall-through
    case CC_PUSH_BUTTON:
      _data2 = 127;
      channelMessage(_status, _data1, _data2);
      break;
    }
  }

  void drag(Position& pos){
    if(_type == HORIZONTAL_CC){
      _data2 = 127*(pos.x-_from_x)/(_to_x-_from_x);
      channelMessage(_status, _data1, _data2);
    }else if(_type == VERTICAL_CC){
      _data2 = 127*(pos.y-_from_y)/(_to_y-_from_y);
      channelMessage(_status, _data1, _data2);
    }
//     switch(_type){
//     case HORIZONTAL_CC:
//       _data2 = (uint8_t)((uint16_t)((pos.x-_from_x)*127)/(_to_x-_from_x));
//       channelMessage(_status, _data1, _data2);
//       break;
//     case VERTICAL_CC:
//       _data2 = (uint8_t)((uint16_t)((pos.y-_from_y)*127)/(_to_y-_from_y));
//       channelMessage(_status, _data1, _data2);
//       break;
//     }
  }

  void tap(Position& pos);

  void tick();

};

#endif /* _MIDIZONE_H_ */
