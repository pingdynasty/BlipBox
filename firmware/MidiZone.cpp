#include <avr/eeprom.h>
#include "MidiZone.h"
#include "globals.h"
#include "serial.h"

// 4 bits
// note/cc button/slider horizontal-momentary/vertical-toggle inverted

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
#define DOT_DISPLAY_TYPE              0x01
#define BAR_DISPLAY_TYPE              0x02
#define FILL_DISPLAY_TYPE             0x03

#define NRPN_PARAMETER_MSB_CC         99
#define NRPN_PARAMETER_LSB_CC         98
#define NRPN_VALUE_MSB_CC             6
#define NRPN_VALUE_LSB_CC             38

#define MIDI_NOTE_OFF                 0x80
#define MIDI_NOTE_ON                  0x90
#define MIDI_AFTERTOUCH               0xA0
#define MIDI_CONTROL_CHANGE           0xB0
#define MIDI_PROGRAM_CHANGE           0xC0
#define MIDI_CHANNEL_PRESSURE         0xD0
#define MIDI_PITCH_BEND               0xE0
#define MIDI_SYSTEM_MESSAGE           0xF0

  // for buttons the on/off values are determined by _min and _max

bool MidiZone::doDrag(){
  return !(_type & BUTTON_SLIDER_ZONE_BIT);
}

bool MidiZone::doPress(){
  return _type & BUTTON_SLIDER_ZONE_BIT;
}

bool MidiZone::doRelease(){
  return (_type & BUTTON_SLIDER_ZONE_BIT) &&
         (_type & MOMENTARY_TOGGLE_ZONE_BIT);
}

float MidiZone::scale(Position& pos){
  float p;
  if(HORIZONTAL_VERTICAL_ZONE_BIT)
    p = ((pos.x-(_from_column*1023.0/10.0))/((_to_column-_from_column)*1023.0/10.0));
  else
    p = ((pos.y-(_from_row*1023.0/8.0))/((_to_row-_from_row)*1023.0/8.0));
  if(_type & INVERTED_ZONE_BIT)
    p = 1.0-p;
  return p;
}

uint8_t MidiZone::scale7(Position& pos) {
  return ((uint8_t)(scale(pos)*(_max - _min)) + _min) & 0x7f;
}

uint16_t MidiZone::scale14(Position& pos) {
  return (uint16_t)(scale(pos) * (_max - _min)*127.0) + _min*127;
}

void MidiZone::drag(Position& pos){
  // only called by slider zones
  uint8_t data = scale7(pos);
  if(data != _data2){
    _data2 = data;
    sendMessage(_data1, _data2);
  }
}

void MidiZone::press(Position& pos){
  // only called by button zones
  if(_type & MOMENTARY_TOGGLE_ZONE_BIT){
    _data2 = _max;
    sendMessage(_data1, _data2);
  }else{
    _data2 = _data2 == _min ? _max : _min;
    sendMessage(_data1, _data2);
  }
}

void MidiZone::release(Position& pos){
  // only called by momentary button zones
  _data2 = _min;
  sendMessage(_data1, _data2);
}

class PitchBendZone : public MidiZone {
  void drag(Position& pos){
    uint16_t data = scale14(pos);
    if(((uint8_t)data & 0x7f) != _data2){ // LSB changed
      _data2 = data & 0x7f;
      sendMessage((data>>7) & 0x7f, _data2);
    }
  }
};

class NRPNZone : public PitchBendZone {
  void sendMessage(uint8_t data1, uint8_t data2){
    sendMessage(NRPN_PARAMETER_MSB_CC, blipbox.config.nrpn_parameter_msb);
    sendMessage(NRPN_PARAMETER_LSB_CC, _data1);
    sendMessage(NRPN_VALUE_MSB_CC, data1);
    if(data2)
      sendMessage(NRPN_VALUE_LSB_CC, data2);
  }
  void press(Position& pos){
    if(_type & MOMENTARY_TOGGLE_ZONE_BIT){
      _data2 = _max;
      sendMessage(_data2, 0);
    }else{
      _data2 = _data2 == _min ? _max : _min;
      sendMessage(_data2, 0);
    }
  }
  void release(Position& pos){
    if((_type & MOMENTARY_TOGGLE_ZONE_BIT)){
      _data2 = _min;
      sendMessage(_data2, 0);
    }
  }
};

class SelectorZone : public MidiZone {
  void press(Position& pos){
    if(blipbox.midizones.preset != _data1)
      blipbox.midizones.loadPreset(_data1);
  }
};

class OneByteMessageZone : public MidiZone {
  void sendMessage(uint8_t data1, uint8_t data2){
    serialWrite(_status);
  }
};

// used by channel after touch, program change
class TwoByteMessageZone : public MidiZone {
  void sendMessage(uint8_t data1, uint8_t data2){
    serialWrite(_status);
    serialWrite(data2 & 0x7f);
  }
};

class ProgramChangeZone : public TwoByteMessageZone {
  // no release / zero message is sent
  void release(Position& pos){}
};

// class ProgramChangeZone : public MidiZone {
//   // no release / zero message is sent
//   void release(Position& pos){}
//   void sendMessage(uint8_t data1, uint8_t data2){
//     serialWrite(_status);
//     serialWrite(data2 & 0x7f);
//   }
// };

// class ChannelAftertouchZone : public MidiZone {
//   void drag(Position& pos){
//     uint8_t data = scale7(pos);
//     if(data != _data2){
//       _data2 = data;
//       sendMessage(_data2);
//     }
//   }
// };

class NoteZone : public MidiZone {
  // nb: if NoteZone is a button, then _data1 is the (fixed) note pitch
  //     if NoteZone is a slider, then _data1 is the (fixed) velocity
  bool doRelease(){
    return true;
  }
  void release(Position& pos){
    if(_type & BUTTON_SLIDER_ZONE_BIT)
      MidiZone::release(pos);
    else
      sendMessage(_data2, 0);
  }
  void drag(Position& pos){
    uint8_t data = scale7(pos);
    if(data != _data2){
      // turn previous note off
      sendMessage(_data2, 0);
      _data2 = data;
      sendMessage(_data2, _data1);
    }
  }
};

// void MidiZone::sendMessage(uint8_t data1){
//   serialWrite(_status);
//   serialWrite(data1 & 0x7f);
// }

void MidiZone::sendMessage(uint8_t data1, uint8_t data2){
  serialWrite(_status);
  serialWrite(data1 & 0x7f);
  serialWrite(data2 & 0x7f);
}

// http://en.wikipedia.org/wiki/Placement_syntax
void * operator new (size_t, void * p) { return p ; }

void MidiZone::read(const uint8_t* data){
  _type   = data[0];
  _data1  = data[1];
//   _data2  = data[2];
  _status = data[2];
  _min    = data[3];
  _max    = data[4];
  _from_column = data[5] >> 4;
  _from_row    = data[5] & 0x0f;
  _to_column   = data[6] >> 4;
  _to_row      = data[6] & 0x0f;

  switch(_type & ZONE_TYPE_MASK){
  case SELECTOR_ZONE_TYPE:
    new(this)SelectorZone();
    break;
  case NRPN_ZONE_TYPE:
    new(this)NRPNZone();
    break;
  case MIDI_ZONE_TYPE:
    switch(_status & 0xf0){
    case MIDI_NOTE_ON:
    case MIDI_NOTE_OFF:
      new(this)NoteZone();
      break;
    case MIDI_AFTERTOUCH:
    case MIDI_CONTROL_CHANGE:
      new(this)MidiZone();
      break;
    case MIDI_PROGRAM_CHANGE:
      new(this)ProgramChangeZone();
      break;
    case MIDI_CHANNEL_PRESSURE:
      new(this)TwoByteMessageZone();
      break;
    case MIDI_PITCH_BEND:
      new(this)PitchBendZone();
      break;
    case MIDI_SYSTEM_MESSAGE:
      new(this)OneByteMessageZone();
      break;
    }
    break;
  }
}

void MidiZone::write(uint8_t* data){
  data[0] = (_type << 4) | (_status & 0x0f);
  data[1] = _data1;
  data[2] = _status;
  data[3] = _min;
  data[4] = _max;
  data[5] = (_from_column << 4) | _from_row;
  data[6] = (_to_column << 4) | _to_row;
}

// void MidiZone::save(uint8_t index){
//   index = MIDI_PRESET_OFFSET+index*MIDI_ZONE_PRESET_SIZE;
//   uint8_t buf[MIDI_ZONE_PRESET_SIZE];
//   write(buf);
//   eeprom_write_block(buf, (uint8_t*)index, sizeof(buf));
// }

void MidiZone::load(uint8_t index){
  index = MIDI_PRESET_OFFSET+index*MIDI_ZONE_PRESET_SIZE;
  uint8_t buf[MIDI_ZONE_PRESET_SIZE];
  eeprom_read_block(buf, (uint8_t*)index, sizeof(buf));
  read(buf);
}

// uint8_t MidiZone::scalex(Position& pos){
// // min = _from_column*1023.0/10.0
// // max = _to_column*1023.0/10.0
// // val = (pos.x-min)/(max-min)*127.0

//   float p = ((pos.x-(_from_column*1023.0/10.0))/((_to_column-_from_column)*1023.0/10.0));
//   if(_type & INVERTED_ZONE_BIT)
//     p = 1.0-p;
//   return (uint8_t)p;
  
// //   float p = 127.0*((pos.x-(_from_column*1023.0/10.0))/((_to_column-_from_column)*1023.0/10.0));
// //   if(_type & INVERTED_ZONE_BIT)
// //     p = 127.0-p;
// //   return (uint8_t)p;
// }

// uint8_t MidiZone::scaley(Position& pos){
//   float p = ((pos.y-(_from_row*1023.0/8.0))/((_to_row-_from_row)*1023.0/8.0));
//   if(_type & INVERTED_ZONE_BIT)
//     p = 1.0-p;
// //   float p = 127.0*((pos.y-(_from_row*1023.0/8.0))/((_to_row-_from_row)*1023.0/8.0));
// //   if(_type & INVERTED_ZONE_BIT)
// //     p = 127.0-p;
//   return (uint8_t)p;
// }

// uint8_t MidiZone::getx(){
// //   return _data2*10/127;
//   uint8_t d = (_type & NOTE_CC_ZONE_BIT) ? _data1 : _data2;
//   d = d*(_to_column-_from_column)/127+_from_column;
//   if(_type & INVERTED_ZONE_BIT)
//     d = _to_column-d-1;
//   return d;
// }

// uint8_t MidiZone::gety(){
// //   return _data2*8/127;
//   uint8_t d = (_type & NOTE_CC_ZONE_BIT) ? _data1 : _data2;
//   d = d*(_to_row-_from_row)/127+_from_row;
//   if(_type & INVERTED_ZONE_BIT)
//     d = _to_row-d-1;
//   return d;
// }

uint8_t MidiZone::getx(){
  uint8_t d = _data2;
  d = d*(_to_column-_from_column)/127+_from_column;
  if(_type & INVERTED_ZONE_BIT)
    d = _to_column-d-1;
  return d;
}

uint8_t MidiZone::gety(){
  uint8_t d = _data2;
  d = d*(_to_row-_from_row)/127+_from_row;
  if(_type & INVERTED_ZONE_BIT)
    d = _to_row-d-1;
  return d;
}

void MidiZone::tick(){
  uint8_t brightness = blipbox.config.brightness;
  if(!(blipbox.keys.isPressed() && check(blipbox.keys.pos)))
    brightness -= brightness/3;
  switch(_type & DISPLAY_TYPE_MASK){
  case DOT_DISPLAY_TYPE:
    break;
  case BAR_DISPLAY_TYPE:
    if(_type & HORIZONTAL_VERTICAL_ZONE_BIT){
      int8_t col = getx();
      for(int8_t row=_from_row; row<_to_row; ++row)
	blipbox.leds.setLed(col, row, brightness);
    }else{
      int8_t row = gety();
      for(int8_t col=_from_column; col<_to_column; ++col)
	blipbox.leds.setLed(col, row, brightness);
    }
    break;
  case FILL_DISPLAY_TYPE:
    if(_data2 && _data2 != _min){
      for(int8_t x=_from_column; x<_to_column; ++x)
	for(int8_t y=_from_row; y<_to_row; ++y)
	  blipbox.leds.setLed(x, y, brightness);
    }
    break;
  case NO_DISPLAY_TYPE:
  default:
    break;
  }
}
