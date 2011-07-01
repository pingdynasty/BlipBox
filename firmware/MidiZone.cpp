#include <wiring.h>
#include <avr/eeprom.h>
#include "MidiZone.h"
#include "globals.h"

#define NOTE_ZONE_BIT      1<<2
#define BUTTON_ZONE_BIT    1<<3
#define HORIZONTAL_CC      1
#define VERTICAL_CC        2
#define CC_PUSH_BUTTON     1 | BUTTON_ZONE_BIT
#define CC_TOGGLE_BUTTON   2 | BUTTON_ZONE_BIT
#define HORIZONTAL_NOTE    1 | NOTE_ZONE_BIT
#define VERTICAL_NOTE      2 | NOTE_ZONE_BIT
#define NOTE_PUSH_BUTTON   1 | NOTE_ZONE_BIT | BUTTON_ZONE_BIT
#define NOTE_TOGGLE_BUTTON 2 | NOTE_ZONE_BIT | BUTTON_ZONE_BIT
#define PRESET_BUTTON      0

void MidiZone::channelMessage(uint8_t data1, uint8_t data2){
  if(data1 != _data1 || data2 != _data2){
    _data1 = data1;
    _data2 = data2;
    serialWrite(_status);
    serialWrite(_data1 & 0x7f);
    serialWrite(_data2 & 0x7f);
  }
}

void MidiZone::read(const uint8_t* data){
  _type =   (data[0] >> 4);
  _status = data[0] & 0x0f; // set channel
  _data1 =  data[1];
  _from_x = data[2] >> 4;
  _from_y = data[2] & 0x0f;
  _to_x =   data[3] >> 4;
  _to_y =   data[3] & 0x0f;
  _data2 = 0;
  if(_type & NOTE_ZONE_BIT)
    _status |= 0x90;
  else
    _status |= 0xb0;
}

void MidiZone::write(uint8_t* data){
  data[0] = (_type << 4) | (_status & 0x0f);
  data[1] = _data1;
  data[2] = (_from_x << 4) | _from_y;
  data[3] = (_to_x << 4) | _to_y;
}

void MidiZone::save(uint8_t index){
  index = MIDI_PRESET_OFFSET+index*MIDI_ZONE_PRESET_SIZE;
  uint8_t buf[MIDI_ZONE_PRESET_SIZE];
  write(buf);
  eeprom_write_block(buf, (uint8_t*)index, sizeof(buf));
}

void MidiZone::load(uint8_t index){
  index = MIDI_PRESET_OFFSET+index*MIDI_ZONE_PRESET_SIZE;
  uint8_t buf[MIDI_ZONE_PRESET_SIZE];
  eeprom_read_block(buf, (uint8_t*)index, sizeof(buf));
  read(buf);
}

uint8_t scalex(Position& pos){
  return pos.x >> 3;
  //     return ((int16_t)127*(pos.column-_from_x))/(_to_x-_from_x);
}

uint8_t scaley(Position& pos){
  return pos.y >> 3;
  //     return ((int16_t)127*(pos.row-_from_y))/(_to_y-_from_y);
}

uint8_t MidiZone::getx(){
  return _data2*10/127;
  //     return (int16_t)_data2*(_to_x-_from_x)/127+_from_x;
}

uint8_t MidiZone::gety(){
  return _data2*8/127;
  //     return (int16_t)_data2*(_to_y-_from_y)/127+_from_y;
}

void MidiZone::release(Position& pos){
//   if(_type & NOTE_ZONE_BIT)
//     _status = 0x80 | (_status & 0x0f);
  if(_type == NOTE_PUSH_BUTTON || _type == CC_PUSH_BUTTON)
    channelMessage(_data1, 0);
}

void MidiZone::press(Position& pos){
//   if(_type & NOTE_ZONE_BIT)
//     _status = 0x90 | (_status & 0x0f);
  if(_type & BUTTON_ZONE_BIT){
    if(_type == NOTE_PUSH_BUTTON || _type == CC_PUSH_BUTTON){
      channelMessage(_data1, 127);
    }else{
      channelMessage(_data1, _data2 == 0 ? 127 : 0);
    }
  }else if(_type == PRESET_BUTTON){
    if(blipbox.midizones.preset != _data1)
      blipbox.midizones.loadPreset(_data1);
  }
}

void MidiZone::drag(Position& pos){
  switch(_type){
  case HORIZONTAL_CC:
    channelMessage(_data1, scalex(pos));
    break;
  case VERTICAL_CC:
    channelMessage(_data1, scaley(pos));
    break;
  case HORIZONTAL_NOTE:
    channelMessage(scalex(pos), scaley(pos));
    break;
  case VERTICAL_NOTE:
    channelMessage(scaley(pos), scalex(pos));
    break;
//   default:
  }
}

void MidiZone::tick(){
  switch(_type){
  case HORIZONTAL_CC:
  case VERTICAL_NOTE:
    int8_t x = getx();
    for(int i=_from_y; i<_to_y; ++i)
      blipbox.leds.setLed(x, i, blipbox.config.brightness);
    break;
  case VERTICAL_CC:
  case HORIZONTAL_NOTE:
    int8_t y = gety();
    for(int i=_from_x; i<_to_x; ++i)
      blipbox.leds.setLed(i, y, blipbox.config.brightness);
    break;
  case NOTE_TOGGLE_BUTTON:
  case CC_TOGGLE_BUTTON:
  case NOTE_PUSH_BUTTON:
  case CC_PUSH_BUTTON:
    if(_data2)
      for(int x=_from_x; x<_to_x; ++x)
	for(int y=_from_y; y<_to_y; ++y)
	  blipbox.leds.setLed(x, y, blipbox.config.brightness);
    break;
  case PRESET_BUTTON:
    if(blipbox.midizones.preset == _data1)
      for(int x=_from_x; x<_to_x; ++x)
	for(int y=_from_y; y<_to_y; ++y)
	  blipbox.leds.setLed(x, y, blipbox.config.brightness);
    break;
  }
}
