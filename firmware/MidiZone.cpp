#include <avr/eeprom.h>
#include "MidiZone.h"
#include "globals.h"
#include "serial.h"

// #define HORIZONTAL_CC      0x1
// #define VERTICAL_CC        0x2
// #define CC_PUSH_BUTTON     0x9
// etc

// 4 bits
// note/cc button/slider horizontal-momentary/vertical-toggle inverted

#define NOTE_CC_ZONE_BIT             (1<<1)
#define BUTTON_SLIDER_ZONE_BIT       (1<<2)
#define HORIZONTAL_VERTICAL_ZONE_BIT (1<<3)
#define MOMENTARY_TOGGLE_ZONE_BIT    (1<<3)
#define INVERTED_ZONE_BIT            (1<<0)

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
  _type =   data[0] >> 4;
  _status = data[0] & 0x0f; // set channel
  _data1 =  data[1];
  _from_x = data[2] >> 4;
  _from_y = data[2] & 0x0f;
  _to_x =   data[3] >> 4;
  _to_y =   data[3] & 0x0f;
  _data2 = 0;
  if(_type & NOTE_CC_ZONE_BIT)
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

uint8_t MidiZone::scalex(Position& pos){
// min = _from_x*1023.0/10.0
// max = _to_x*1023.0/10.0
// val = (pos.x-min)/(max-min)*127.0
  float p = 127.0*((pos.x-(_from_x*1023.0/10.0))/((_to_x-_from_x)*1023.0/10.0));
  if(_type & INVERTED_ZONE_BIT)
    p = 127.0-p;
  return (uint8_t)p;
//   pos = pos.x/1023.0; -wrong!
//   min = _from_x/10.0;
//   range = (_to_x-_from_x)/10.0;
//   pos = (pos-min)*range
//   float p = 127.0*(pos.x/1023.0-_from_x/10.0)*((_to_x-_from_x)/10.0);

//   return pos.x >> 3;
//   return ((int16_t)127*(pos.x-_from_x*1023/10)/((_to_x-_from_x)*1023/10));
  //     return ((int16_t)127*(pos.column-_from_x))/(_to_x-_from_x);
}

uint8_t MidiZone::scaley(Position& pos){
  float p = 127.0*((pos.y-(_from_y*1023.0/8.0))/((_to_y-_from_y)*1023.0/8.0));
//   float p = 127.0*(pos.y/1023.0-_from_y/8.0)*((_to_y-_from_y)/8.0);
  if(_type & INVERTED_ZONE_BIT)
    p = 127.0-p;
  return (uint8_t)p;
//   return pos.y >> 3;
  //     return ((int16_t)127*(pos.row-_from_y))/(_to_y-_from_y);
}

uint8_t MidiZone::getx(){
//   return _data2*10/127;
  uint8_t d = (_type & NOTE_CC_ZONE_BIT) ? _data1 : _data2;
  d = d*(_to_x-_from_x)/127+_from_x;
  if(_type & INVERTED_ZONE_BIT)
    d = _to_x-d-1;
  return d;
}

uint8_t MidiZone::gety(){
//   return _data2*8/127;
  uint8_t d = (_type & NOTE_CC_ZONE_BIT) ? _data1 : _data2;
  d = d*(_to_y-_from_y)/127+_from_y;
  if(_type & INVERTED_ZONE_BIT)
    d = _to_y-d-1;
  return d;
}

void MidiZone::press(Position& pos){
//   if(_type & NOTE_ZONE_BIT)
//     _status = 0x90 | (_status & 0x0f);
  if(_type & BUTTON_SLIDER_ZONE_BIT){
    if(_type & MOMENTARY_TOGGLE_ZONE_BIT){
      channelMessage(_data1, 127);
    }else{
      channelMessage(_data1, _data2 == 0 ? 127 : 0);
    }
  }
//   }else if(_type == PRESET_BUTTON){
//     if(blipbox.midizones.preset != _data1)
//       blipbox.midizones.loadPreset(_data1);
//   }
}

void MidiZone::release(Position& pos){
//   if(_type & NOTE_ZONE_BIT)
//     _status = 0x80 | (_status & 0x0f);
  if((_type & BUTTON_SLIDER_ZONE_BIT) &&
     (_type & MOMENTARY_TOGGLE_ZONE_BIT))
    channelMessage(_data1, 0);
//   if(_type == NOTE_PUSH_BUTTON || _type == CC_PUSH_BUTTON)
//     channelMessage(_data1, 0);
}

//  uint8_t MidiZone::getData1(Position& pos){
//    if(_type & NOTE_CC_ZONE_BIT)
//      return (_type & HORIZONTAL_VERTICAL_ZONE_BIT) ?
//        scalex(pos) : scaley(pos);
//    else
//      return _data1;
//  }

//  uint8_t MidiZone::getData2(Position& pos){
//    if(_type & NOTE_CC_ZONE_BIT)
//      return (_type & HORIZONTAL_VERTICAL_ZONE_BIT) ?
//        scaley(pos) : scalex(pos);
//    else
//      return (_type & HORIZONTAL_VERTICAL_ZONE_BIT) ?
//        scalex(pos) : scaley(pos);
//  }

 void MidiZone::drag(Position& pos){
   if(_type & BUTTON_SLIDER_ZONE_BIT)
     return;
   if(_type & NOTE_CC_ZONE_BIT)
     if(_type & HORIZONTAL_VERTICAL_ZONE_BIT)
       channelMessage(scalex(pos), scaley(pos));
     else
       channelMessage(scaley(pos), scalex(pos));
   else
     if(_type & HORIZONTAL_VERTICAL_ZONE_BIT)
       channelMessage(_data1, scalex(pos));
     else
       channelMessage(_data1, scaley(pos));
//   switch(_type){
//   case HORIZONTAL_CC:
//     channelMessage(_data1, scalex(pos));
//     break;
//   case VERTICAL_CC:
//     channelMessage(_data1, scaley(pos));
//     break;
//   case HORIZONTAL_NOTE:
//     channelMessage(scalex(pos), scaley(pos));
//     break;
//   case VERTICAL_NOTE:
//     channelMessage(scaley(pos), scalex(pos));
//     break;
// //   default:
//   }
 }

void MidiZone::tick(){
  if(_type & BUTTON_SLIDER_ZONE_BIT){
    if(_data2){
      for(int8_t x=_from_x; x<_to_x; ++x)
	for(int8_t y=_from_y; y<_to_y; ++y)
	  blipbox.leds.setLed(x, y, blipbox.config.brightness);
    }
  }else if(_type & HORIZONTAL_VERTICAL_ZONE_BIT){
    int8_t x = getx();
    for(int8_t y=_from_y; y<_to_y; ++y)
      blipbox.leds.setLed(x, y, blipbox.config.brightness);
  }else{
    int8_t y = gety();
    for(int8_t i=_from_x; i<_to_x; ++i)
      blipbox.leds.setLed(i, y, blipbox.config.brightness);
  }
//   switch(_type){
//   case HORIZONTAL_CC:
//   case VERTICAL_NOTE:
//     int8_t x = getx();
//     for(int8_t y=_from_y; y<_to_y; ++y)
//       blipbox.leds.setLed(x, y, blipbox.config.brightness);
//     break;
//   case VERTICAL_CC:
//   case HORIZONTAL_NOTE:
//     int8_t y = gety();
//     for(int8_t i=_from_x; i<_to_x; ++i)
//       blipbox.leds.setLed(i, y, blipbox.config.brightness);
//     break;
//   case NOTE_TOGGLE_BUTTON:
//   case CC_TOGGLE_BUTTON:
//   case NOTE_PUSH_BUTTON:
//   case CC_PUSH_BUTTON:
//     if(_data2)
//       for(int8_t x=_from_x; x<_to_x; ++x)
// 	for(int8_t y=_from_y; y<_to_y; ++y)
// 	  blipbox.leds.setLed(x, y, blipbox.config.brightness);
//     break;
//   case PRESET_BUTTON:
//     if(blipbox.midizones.preset == _data1)
//       for(int8_t x=_from_x; x<_to_x; ++x)
// 	for(int8_t y=_from_y; y<_to_y; ++y)
// 	  blipbox.leds.setLed(x, y, blipbox.config.brightness);
//     break;
//   }
}
