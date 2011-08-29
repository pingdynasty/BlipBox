#include <avr/eeprom.h>
#include "MidiZone.h"
#include "globals.h"
#include "serial.h"

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

bool MidiZone::doTick(){
  return (_type & DISPLAY_TYPE_MASK) != NO_DISPLAY_TYPE &&
    _data2 != -1;
}

float MidiZone::scale(Position& pos){
  float p;
  if(_type & HORIZONTAL_VERTICAL_ZONE_BIT)
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

void MidiZone::sendMessage(uint8_t data1, uint8_t data2){
  serialWrite(_status);
  serialWrite(data1 & 0x7f);
  serialWrite(data2 & 0x7f);
}

#ifndef CV_DAC_HACK

class PitchBendZone : public MidiZone {
  void drag(Position& pos){
    uint16_t data = scale14(pos);
    if(((uint8_t)data & 0x7f) != _data1){
      _data1 = data & 0x7f; // LSB 
      _data2 = (data>>7) & 0x7f; // MSB
      sendMessage(_data1, _data2);
    }
  }
};

class NRPNZone : public MidiZone {
  // _data1 is used to store parameter LSB
  void drag(Position& pos){
    uint16_t data = scale14(pos);
    if((((data>>7) & 0x7f)) != _data2){ // comparing MSB only
      _data2 = (data>>7) & 0x7f; // MSB
      sendMessage(_data2, 0);
//       sendMessage(_data2, data & 0x7f);
    }
  }
  void sendMessage(uint8_t data1, uint8_t data2){
    MidiZone::sendMessage(NRPN_PARAMETER_MSB_CC, blipbox.config.nrpn_parameter_msb);
    MidiZone::sendMessage(NRPN_PARAMETER_LSB_CC, _data1);
    MidiZone::sendMessage(NRPN_VALUE_MSB_CC, data1);
    if(data2)
      MidiZone::sendMessage(NRPN_VALUE_LSB_CC, data2);
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
      sendMessage(_data2, _min);
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

class NoteZone : public MidiZone {
  // nb: if NoteZone is a button, then _data1 is the (fixed) note pitch
  //     if NoteZone is a slider, then _data1 is the (fixed) velocity
  bool doRelease(){
    return true;
  }
  void release(Position& pos){
    if(_type & BUTTON_SLIDER_ZONE_BIT){
      if((_type & MOMENTARY_TOGGLE_ZONE_BIT) && _data1 != 0){
	_data2 = 0;
	sendMessage(_data1, 0);
      }
    }else if(_data2 != -1){
      sendMessage(_data2, 0);
      _data2 = -1;
    }
  }
  void drag(Position& pos){
    uint8_t data = scale7(pos);
    if(data != _data2){
      // turn previous note off
      if(_data2 != -1)
	sendMessage(_data2, 0);
      _data2 = data;
      sendMessage(_data2, _data1);
    }
  }
};

#endif // CV_DAC_HACK

#ifdef CV_DAC_HACK
#include "spi.h"
#define DAC_A_B_BIT  7 // 1 = Write to A, 0 = Write to B
#define DAC_BUF_BIT  6 // 1 = Buffered, 0 = Unbuffered
#define DAC_GA_BIT   5 // 1 = 1x Gain, 0 = 2x Gain
#define DAC_SHDN_BIT 4 // 1 = Output buffer enabled, 0 = Output buffer disabled
#define DAC_TRANSFER_BITS _BV(DAC_SHDN_BIT)

class CV_DAC_HACK_Zone : public MidiZone {
  void drag(Position& pos){
    uint16_t data = scale14(pos);
    if(((uint8_t)data & 0x7f) != _data1){
      _data1 = data & 0x7f; // LSB 
      _data2 = (data>>7) & 0x7f; // MSB
      sendMessage(_data1, _data2);
    }
  }
  void press(Position& pos){
    _data1 = 127;
    MidiZone::press(pos);
  }
  void release(Position& pos){
    _data1 = 0;
    MidiZone::release(pos);
  }
  void sendMessage(uint8_t data1, uint8_t data2){
    spi_cs_toggle();
    spi_cs(_status & 0x01);
    data1 = ((data2 << 5) & 0xff) | (data1 >> 2);
    data2 = (data2 >> 3) & 0x0f;
    data2 |= DAC_TRANSFER_BITS;
    if(_status & 0x02)
      data2 |= _BV(DAC_A_B_BIT);
    spi_send(data2);
    spi_send(data1);
    spi_cs_toggle();
  }
};

#endif // CV_DAC_HACK

// see http://en.wikipedia.org/wiki/Placement_syntax
void * operator new (size_t, void * p); // defined in operators.cpp

void MidiZone::read(const uint8_t* data){
  _type   = data[0];
  _data1  = data[1];
  _status = data[2];
  _min    = data[3];
  _max    = data[4];
  _from_column = data[5] >> 4;
  _from_row    = data[5] & 0x0f;
  _to_column   = data[6] >> 4;
  _to_row      = data[6] & 0x0f;
  _data2 = -1;

#ifdef CV_DAC_HACK
  new(this)CV_DAC_HACK_Zone();
#endif // CV_DAC_HACK

#ifndef CV_DAC_HACK
  switch(_type & ZONE_TYPE_MASK){
  case SELECTOR_ZONE_TYPE:
    new(this)SelectorZone();
    break;
  case NRPN_ZONE_TYPE:
    new(this)NRPNZone();
    break;
  case MIDI_ZONE_TYPE:
    switch(_status & MIDI_STATUS_MASK){
    case MIDI_AFTERTOUCH:
    case MIDI_CONTROL_CHANGE:
      new(this)MidiZone();
      break;
    case MIDI_NOTE_ON:
    case MIDI_NOTE_OFF:
      new(this)NoteZone();
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
#endif // CV_DAC_HACK
}

void MidiZone::write(uint8_t* data){
  data[0] = _type;
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

void MidiZone::load(uint8_t* address){
  uint8_t buf[MIDI_ZONE_PRESET_SIZE];
  eeprom_read_block(buf, address, sizeof(buf));
  read(buf);
}

uint8_t MidiZone::getx(){
//   if(_data2 < _min || _max == _min)
//     return _min;
//   uint8_t d = _data2 > _min ? _data2 : _min; // in case _data2 == -1
  uint8_t d = _data2;
  d = (d-_min)*(_to_column-_from_column)/(_max-_min)+_from_column;
  if(_type & INVERTED_ZONE_BIT)
    d = _to_column-d-1;
  return d;
}

uint8_t MidiZone::gety(){
//   if(_data2 < _min || _max == _min)
//     return _min;
//   uint8_t d = _data2 > _min ? _data2 : _min; // in case _data2 == -1
  uint8_t d = _data2;
  d = (d-_min)*(_to_row-_from_row)/(_max-_min)+_from_row;
  if(_type & INVERTED_ZONE_BIT)
    d = _to_row-d-1;
  return d;
}

void MidiZone::tick(){
  uint8_t brightness = blipbox.config.brightness;
//   if(!(blipbox.keys.isPressed() && check(blipbox.keys.pos)))
//     brightness -= brightness/3;
  switch(_type & DISPLAY_TYPE_MASK){
  case DOT_DISPLAY_TYPE:
    blipbox.leds.setLed(getx(), gety(), brightness);
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
    if(!(_type & BUTTON_SLIDER_ZONE_BIT)){
      uint8_t fy = _from_row;
      uint8_t ty = _to_row;
      uint8_t fx = _from_column;
      uint8_t tx = _to_column;
      if(_type & HORIZONTAL_VERTICAL_ZONE_BIT){
	if(_type & INVERTED_ZONE_BIT)
	  fx = getx();
	else
	  tx = getx()+1;
      }else{
	if(_type & INVERTED_ZONE_BIT)
	  fy = gety();
	else
	  ty = gety()+1;
      }
      for(int8_t x=fx; x<tx; ++x)
	for(int8_t y=fy; y<ty; ++y)
	  blipbox.leds.setLed(x, y, brightness);
    }else if(_data2 != _min){
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
