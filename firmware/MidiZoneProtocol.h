#ifndef _MIDIZONEPROTOCOL_H_
#define _MIDIZONEPROTOCOL_H_

#include <avr/eeprom.h>
#include <inttypes.h>
#include "Protocol.h"

#define ZONES 8
#define PRESET_OFFSET 32 // arbitrary bytes for configuration

// ATMega168 has 512 bytes EEPROM
// each zone takes 11 data bytes (+4 bytes if there are virtual functions in the class)
// each preset takes 11*8=88 bytes, room for 5 presets / 440 bytes

enum MidiZoneType {
  HORIZONTAL_CC, VERTICAL_CC, HORIZONTAL_NOTE, VERTICAL_NOTE
};

class MidiZone {
private:
  MidiZoneType _type;
  uint8_t _status;
  uint8_t _data1;
  uint16_t _from_x, _to_x;
  uint16_t _from_y, _to_y;
  inline void channelMessage(uint8_t status, uint8_t data1, uint8_t data2){
    serialWrite(status);
    serialWrite(data1 & 0x7f);
    serialWrite(data2 & 0x7f);
  }
public:
  void write(uint8_t index){
    index = PRESET_OFFSET+index*sizeof(*this);
    uint8_t* p = (uint8_t*)(void*)this;
    for(uint8_t i = index; i < sizeof(*this)+index; i++)
      eeprom_write_byte((uint8_t*)i, *p++);
  }
  void read(uint8_t index){
    index = PRESET_OFFSET+index*sizeof(*this);
    uint8_t* p = (uint8_t*)(void*)this;
    for(uint8_t i = index; i < sizeof(*this)+index; i++)
      *p++ = eeprom_read_byte((uint8_t*)i);
  }
/*   void setCCMode(uint8_t channel, uint8_t cc, bool horizontal){ */
/*     _status = 0xb0 | channel; */
/*     _data1 = cc; */
/*     _type = horizontal ? HORIZONTAL_CC : VERTICAL_CC; */
/*   } */
/*   void setNoteMode(uint8_t channel, bool horizontal){ */
/*     _status = 0x80 | channel; */
/*     _type = horizontal ? HORIZONTAL_NOTE : VERTICAL_NOTE; */
/*   } */
  bool check(uint16_t x, uint16_t y){
    return _from_x<=x && x<_to_x && _from_y<=y && y<_to_y;
  }
  void sendReleaseMessage(){
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
  void sendPositionMessage(uint16_t x, uint16_t y){
    x = (uint16_t)((x-_from_x) * 127.0 / (_to_x-_from_x));
    y = (uint16_t)((y-_from_y) * 127.0 / (_to_y-_from_y));
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

class MidiZoneProtocol : public Protocol {
private:
  MidiZone zones[ZONES];
public:
  void loadPreset(int index){
    index = index*sizeof(zones[0])*ZONES;
    for(int i=0; i<ZONES; ++i)
      zones[i].read(i+index);
  }
  void savePreset(int index){
    index = index*sizeof(zones[0])*ZONES;
    for(int i=0; i<ZONES; ++i)
      zones[i].write(i);
  }
  void sendParameterMessage(uint8_t pid, uint16_t value){}
  void sendSensorMessage(uint8_t pid, uint16_t value){}
  void sendPositionMessage(uint16_t x, uint16_t y){
    for(int i=0; i<ZONES; ++i)
      if(zones[i].check(x, y))
        zones[i].sendPositionMessage(x, y);
  }
  void sendReleaseMessage(){
    for(int i=0; i<ZONES; ++i)
/*       if(zones[i].check(x, y)) TODO: check x/y of zones */ 
        zones[i].sendReleaseMessage();
  }
};

#endif /* _MIDIZONEPROTOCOL_H_ */
