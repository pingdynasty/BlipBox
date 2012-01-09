#ifndef _ACTION_H_
#define _ACTION_H_

#include <inttypes.h>
#include "MidiConstants.h"
#include "Event.h"

#define MIN_DATA_VALUE 0.0
#define MAX_DATA_VALUE 1.0

// status codes 0x0x to 0x7x (top bit zero) used for non-midi actions
#define NO_ACTION_TYPE                 0x00
#define SELECT_PRESET_ACTION_TYPE      0x10 // preset number in lower half
#define MIDI_NRPN_ACTION_TYPE          0x20 // 
#define CONTROL_VOLTAGE_ACTION_TYPE    0x30 // cv channel in lower half

class Action {
public:
//   virtual ~Action(){}
  virtual void on(float data){}
  virtual void off(){}
  virtual float getValue() { return MIN_DATA_VALUE; }
  virtual uint8_t getType() { return NO_ACTION_TYPE; }
  virtual uint8_t read(const uint8_t* data) { return 0; }
  virtual uint8_t write(uint8_t* data) { return 0; }
  virtual void handle(MidiEvent& event) {}
  static Action* createAction(uint8_t type);
};

class AbstractAction : public Action {
public:
  uint8_t status;
  uint8_t minimum;
  uint8_t maximum;
  AbstractAction(uint8_t astatus) :
    status(astatus), minimum(0), maximum(128) {}
  float scaleFrom7(int8_t value){
    return ((float)value-minimum)/(maximum-minimum);
  }
  float scaleFrom14(int16_t value){
    return ((float)value-minimum*0x7f)/((maximum-minimum)*0x7f);
  }
  int16_t constrain(int16_t value);
  int8_t constrain(int8_t value);
  int8_t scaleTo7(float value){
    return (int8_t)(value*(maximum-minimum))+minimum;
  }
  int16_t scaleTo14(float value){
    return (int16_t)(value*(maximum-minimum)*0x7f)+minimum*0x7f;
  }
  uint8_t getType() { 
    return status;
  }
  void setStatus(uint8_t value){
    status = (value & MIDI_STATUS_MASK) | (status & MIDI_CHANNEL_MASK);
  }
  uint8_t getStatus(){
    return (status & MIDI_STATUS_MASK);
  }
  void setChannel(uint8_t value){
    status = (status & MIDI_STATUS_MASK) | (value & MIDI_CHANNEL_MASK);
  }
  uint8_t getChannel(){
    return (status & MIDI_CHANNEL_MASK);
  }
  virtual uint8_t read(const uint8_t* data){
    status  = data[3];
    minimum = data[4];
    maximum = data[5];
    return 3;
  }
  virtual uint8_t write(uint8_t* data){
    data[3] = status;
    data[4] = minimum;
    data[5] = maximum;
    return 3;
  }
};

class MidiAction : public AbstractAction {
public:
  MidiAction(uint8_t status) :
    AbstractAction(status) {}
  void sendMessage(uint8_t data1, int8_t data2);
  void sendMessage(int16_t data);
  void sendMessage(int8_t data);
};

class AbstractMidiAction : public MidiAction {
public:
  uint8_t data1;
  AbstractMidiAction(uint8_t status, uint8_t d1) :
    MidiAction(status), data1(d1) {}
  uint8_t read(const uint8_t* data){
    data1   = data[6];
    return AbstractAction::read(data)+1;
  }
  uint8_t write(uint8_t* data){
    data[6] = data1;
    return AbstractAction::write(data)+1;
  }
};

class MidiControllerAction : public AbstractMidiAction {
public:
  int8_t data2;
  MidiControllerAction() : AbstractMidiAction(MIDI_CONTROL_CHANGE, 0), data2(0) {}
/*   MidiControllerAction(uint8_t astatus, uint8_t amin, uint8_t amax, uint8_t d1, uint8_t d2) :  */
/*     MidiAction(astatus, amin, amax), data1(d1), data2(d2) {} */
  void on(float data){
    uint8_t value = scaleTo7(data);
    if(data2 != value){
      data2 = value;
      sendMessage(data1, data2);
    }
  }
  float getValue(){
    return scaleFrom7(data2);
  }
  void handle(MidiEvent& event){
    if(event.getType() == status && event.getData1() == data1)
      data2 = constrain((int8_t)event.getData2());
  }
};

class MidiNoteAction : public AbstractMidiAction {
public:
  int8_t pitch;
  MidiNoteAction() : AbstractMidiAction(MIDI_NOTE_ON, 80), pitch(-1) {}
  // pitch is sent as pitch
  // data1 is sent as velocity
  void off(){
    if(pitch != -1)
      sendMessage(pitch, 0); // send note off    
    pitch = -1;
  }
  void on(float data){
    uint8_t value = scaleTo7(data);
    if(pitch != value){
      off(); // send note off
      pitch = value;
      sendMessage(pitch, data1); // send note on
    }
  }
  float getValue(){
    return pitch == -1 ? 0 : scaleFrom7(pitch);
  }
  void handle(MidiEvent& event){
    if(event.getChannel() == getChannel()){
      if(event.isNoteOn()){
	if(event.getData2() == 0){
	  pitch = -1;
	}else{
	  pitch = constrain((int8_t)event.getData1());
	}
      }else if(event.isNoteOff()){
	pitch = -1;
      }
    }
  }
};

class MidiPitchBendAction : public MidiAction {
public:
  int16_t data;
  MidiPitchBendAction() : MidiAction(MIDI_PITCH_BEND), data(8192) {}
  void on(float f){
    int16_t value = scaleTo14(f);
    if(data != value){
      data = value;
      sendMessage(data);
    }
  }
  float getValue(){
    return scaleFrom14(data);
  }
  void handle(MidiEvent& event){
    if(event.getType() == status)
      data = constrain((int16_t)event.getData());
  }
};

class MidiChannelPressureAction : public MidiAction {
public:
  int8_t data;
  MidiChannelPressureAction() : MidiAction(MIDI_CHANNEL_PRESSURE), data(0) {}
  void on(float f){
    int8_t value = scaleTo7(f);
    if(data != value){
      data = value;
      sendMessage(data);
    }
  }
  float getValue(){
    return scaleFrom7(data);
  }
};

#endif /* _ACTION_H_ */
