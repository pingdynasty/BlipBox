#ifndef _MIDIZONEEVENTHANDLER_H_
#define _MIDIZONEEVENTHANDLER_H_

#include <wiring.h>
#include <avr/eeprom.h>
#include <inttypes.h>
#include "EventHandler.h"

#define MIDI_ZONES 8
#define MIDI_PRESET_OFFSET 32 // arbitrary 32 bytes for configuration
#define MIDI_PRESET_SIZE 4

// ATMega168 has 512 bytes EEPROM

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
  uint8_t _data1;
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
    index = MIDI_PRESET_OFFSET+index*MIDI_PRESET_SIZE;
    uint8_t buf[MIDI_PRESET_SIZE];
    write(buf);
    eeprom_write_block(buf, (uint8_t*)index, sizeof(buf));
  }
  void load(uint8_t index){
    index = MIDI_PRESET_OFFSET+index*MIDI_PRESET_SIZE;
    uint8_t buf[MIDI_PRESET_SIZE];
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
      if(_status & 0xf0 == 0x90){
        _status ^= 0x10;
        channelMessage(_status, _data1, 0);
      }
      break;
    }
  }
  void sendNoteOn(uint8_t pitch, uint8_t velocity){
    if(_status & 0xf0 == 0x80){
      _status ^= 0x10;
      _data1 = pitch;
      channelMessage(_status, _data1, velocity);
    }
  }
  void drag(Position& pos){
    uint8_t x = (uint8_t)((pos.x-_from_x) * 127.0 / (_to_x-_from_x));
    uint8_t y = (uint8_t)((pos.y-_from_y) * 127.0 / (_to_y-_from_y));
    switch(_type){
    case HORIZONTAL_CC:
      channelMessage(_status, _data1, x);
      break;
    case VERTICAL_CC:
      channelMessage(_status, _data1, y);
      break;
    case HORIZONTAL_NOTE:
      sendNoteOn(x, y);
      break;
    case VERTICAL_NOTE:
      sendNoteOn(y, x);
      break;
    }
  }
};

class MidiZoneEventHandler : public EventHandler {
private:
  MidiZone zones[MIDI_ZONES];
  uint8_t preset;
public:
  void init(){}
  void setPreset(uint8_t index){
    preset = index;
  }
  uint8_t getPreset(){
    return preset;
  }
  void loadPreset(int index){
    preset = index;
    index = index*MIDI_PRESET_SIZE*MIDI_ZONES;
    for(int i=0; i<MIDI_ZONES; ++i)
      zones[i].load(i+index);
  }
  void savePreset(int index){
    index = index*MIDI_PRESET_SIZE*MIDI_ZONES;
    for(int i=0; i<MIDI_ZONES; ++i)
      zones[i].save(i);
  }
  MidiZone& getZone(uint8_t zone){
    return zones[zone];
  }
  void press(Position& pos){}
  void release(Position& pos){
    for(int i=0; i<MIDI_ZONES; ++i)
      if(zones[i].check(pos))
        zones[i].release(pos);
  }
  void drag(Position& pos){
    for(int i=0; i<MIDI_ZONES; ++i)
      if(zones[i].check(pos))
        zones[i].drag(pos);
  }
  void tap(Position& pos){}
  void taptap(Position& pos);
};

#endif /* _MIDIZONEEVENTHANDLER_H_ */
