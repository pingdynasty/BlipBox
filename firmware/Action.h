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
#define MIDI_NOTE_RANGE_ACTION_TYPE    0x30 // 
#define CONTROL_VOLTAGE_ACTION_TYPE    0x70 // cv channel in lower half

class Action {
public:
//   virtual ~Action(){}
  virtual void on(float data){}
  virtual void off(){}
  virtual float getValue() { return MIN_DATA_VALUE; }
  virtual uint8_t getActionType() { return NO_ACTION_TYPE; }
  virtual uint8_t read(const uint8_t* data) { return 0; }
  virtual uint8_t write(uint8_t* data) { return 0; }
  virtual void handle(MidiEvent& event) {}
  static Action* createAction(uint8_t type);
};

class MidiAction : public Action {
public:
  uint8_t status;
  uint8_t minimum;
  uint8_t maximum;
  MidiAction(uint8_t astatus) :
    status(astatus), minimum(0), maximum(128) {}
  float scaleFrom7(int8_t value){
    return ((float)value-minimum)/(maximum-minimum);
  }
  float scaleFrom14(int16_t value){
    return ((float)value-minimum*MIDI_VALUE_MASK)/((maximum-minimum)*MIDI_VALUE_MASK);
  }
  int16_t constrain(int16_t value);
  int8_t constrain(int8_t value);
  int8_t scaleTo7(float value){
    return (int8_t)(value*(maximum-minimum))+minimum;
  }
  int16_t scaleTo14(float value){
    return (int16_t)(value*(maximum-minimum)*MIDI_VALUE_MASK)+minimum*MIDI_VALUE_MASK;
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
  virtual uint8_t getActionType() { 
    return (status & MIDI_STATUS_MASK);
  }
  uint8_t read(const uint8_t* data){
    setChannel(data[3]);
    minimum = data[4];
    maximum = data[5];
    return 3;
  }
  uint8_t write(uint8_t* data){
    data[3] = getActionType() | getChannel();
    data[4] = minimum;
    data[5] = maximum;
    return 3;
  }
  void sendMessage(uint8_t data1, int8_t data2);
  void sendMessage(int16_t data);
  void sendMessage(int8_t data);
};

class SelectPresetAction : public Action {
private:
  uint8_t status;
public:
  SelectPresetAction() : status(SELECT_PRESET_ACTION_TYPE) {}
  void on(float data);
  float getValue();
  uint8_t getActionType() { 
    return status & MIDI_STATUS_MASK; 
  }
  uint8_t getPresetIndex(){
    return (status & MIDI_CHANNEL_MASK);
  }
  void setPresetIndex(uint8_t value){
    status = (status & MIDI_STATUS_MASK) | (value & MIDI_CHANNEL_MASK);
  }
  uint8_t read(const uint8_t* data){
    status  = data[3];
    return 1;
  }
  uint8_t write(uint8_t* data){
    data[3] = status;
    return 1;
  }
};

class AbstractMidiAction : public MidiAction {
public:
  uint8_t data1;
  AbstractMidiAction(uint8_t status, uint8_t d1) :
    MidiAction(status), data1(d1) {}
  uint8_t read(const uint8_t* data){
    data1   = data[6];
    return MidiAction::read(data)+1;
  }
  uint8_t write(uint8_t* data){
    data[6] = data1;
    return MidiAction::write(data)+1;
  }
};

class MidiControllerAction : public AbstractMidiAction {
public:
  int8_t data2;
  MidiControllerAction() : AbstractMidiAction(MIDI_CONTROL_CHANGE, 0), data2(0) {}
  MidiControllerAction(uint8_t status) : AbstractMidiAction(status, 0), data2(0) {}
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
      // value of MidiEvent.getType() contains both status and channel
      data2 = constrain((int8_t)event.getData2());
  }
};

class MidiNoteAction : public AbstractMidiAction {
  // sends single notes of variable velocity
public:
  int8_t data2;
  MidiNoteAction() : AbstractMidiAction(MIDI_NOTE_ON, 80), data2(0) {}
  // data1 is sent as pitch
  // data2 is sent as velocity
  void off(){
    if(data2)
      sendMessage(data1, 0); // send note off    
    data2 = 0;
  }
  void on(float data){
    uint8_t value = scaleTo7(data);
    if(data2 != value){
//       off(); // send note off
      data2 = value;
//       if(data2)
      sendMessage(data1, data2); // send note on
    }
  }
  float getValue(){
    return scaleFrom7(data2);
  }
  void handle(MidiEvent& event){
    if(event.getChannel() == getChannel() && event.getData1() == data1){
      if(event.isNoteOn()){
	if(event.getData2() == 0){
	  // same as note off
	  data2 = 0;
	}else{
	  data2 = constrain((int8_t)event.getData2());
	}
      }else if(event.isNoteOff()){
	data2 = 0;
      }
    }
  }
};

class MidiNoteRangeAction : public AbstractMidiAction {
public:
  int8_t pitch;
  MidiNoteRangeAction() : AbstractMidiAction(MIDI_NOTE_ON, 80), pitch(-1) {}
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
  uint8_t getActionType() { return MIDI_NOTE_RANGE_ACTION_TYPE; }
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
  MidiChannelPressureAction(uint8_t status) : MidiAction(status), data(0) {}
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
  void handle(MidiEvent& event){
    if(event.getType() == status)
      data = constrain((int8_t)event.getData1());
  }
};

class MidiProgramChangeAction : public MidiChannelPressureAction {
public:
  MidiProgramChangeAction() : MidiChannelPressureAction(MIDI_PROGRAM_CHANGE) {}
};

class MidiAftertouchAction : public MidiControllerAction {
public:
 MidiAftertouchAction() : MidiControllerAction(MIDI_AFTERTOUCH) {}
};

/* distinct class not needed, taken care of by MidiControllerAction */
/* class MidiAftertouchAction : public AbstractMidiAction { */
/* public: */
/*   int8_t data2; */
/*   MidiAftertouchAction() : MidiAction(MIDI_AFTERTOUCH), data(0) {} */
/*   void on(float f){ */
/*     int8_t value = scaleTo7(f); */
/*     if(data2 != value){ */
/*       data2 = value; */
/*       sendMessage(data1, data2); */
/*     } */
/*   } */
/*   float getValue(){ */
/*     return scaleFrom7(data2); */
/*   } */
/* }; */

class MidiNRPNAction : public MidiAction {
public:
/* First, controller 99 - NRPN Most Significant Bit (MSB) - followed by 98 - NRPN Least Significant Bit (LSB) sent as a pair specify the parameter to be changed. Controller 6 then sets the value of the parameter in question. Controller 38 may optionally then be sent as a fine adjustment to the value set by controller 6. */
  int16_t data1;
  int16_t data2;
  MidiNRPNAction() : MidiAction(MIDI_CONTROL_CHANGE), data1(0), data2(0) {}
  void on(float f){
    int16_t value = scaleTo14(f);
    if(data2 != value){
      data2 = value;
      send(data1, data2);
    }
  }
  float getValue(){
    return scaleFrom14(data2);
  }
  uint8_t getActionType() { return MIDI_NRPN_ACTION_TYPE; }
  void send(uint16_t pid, uint16_t value){
    sendMessage(NRPN_PARAMETER_MSB_CC, data1 >> 7);
    sendMessage(NRPN_PARAMETER_LSB_CC, data1 & MIDI_VALUE_MASK);
    sendMessage(NRPN_VALUE_MSB_CC, data2 >> 7);
    sendMessage(NRPN_VALUE_LSB_CC, data2 & MIDI_VALUE_MASK);
  }
  uint8_t read(const uint8_t* data){
    data1   = data[6] | (data[7] << 7);
    data2   = data[8] | (data[9] << 7);
    return MidiAction::read(data)+4;
  }
  uint8_t write(uint8_t* data){
    data[6] = data1 & MIDI_VALUE_MASK;
    data[7] = (data1 >> 7) & MIDI_VALUE_MASK;
    data[8] = data2 & MIDI_VALUE_MASK;
    data[9] = (data2 >> 7) & MIDI_VALUE_MASK;
    return MidiAction::write(data)+4;
  }
  void handle(MidiEvent& event){
//     if(event.getType() == status && event.getData1() == data1)
    // todo: need to keep state to receive 3-4 message sequence
  }
};

#endif /* _ACTION_H_ */
