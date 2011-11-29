#ifndef _ACTION_H_
#define _ACTION_H_

#include "serial.h"

#define MIN_DATA_VALUE 0.0
#define MAX_DATA_VALUE 1023.0

class Action {
public:
  virtual void setValue(float data){};
  virtual float getValue(){ return MIN_DATA_VALUE; };
/*   virtual void off(){}; */
/*   virtual void on(float data){}; */
/*   virtual void off(){}; */
/*   virtual bool isOn(){ */
/*     return false; */
/*   } */
};

class MidiAction : public Action {
public:
  uint8_t status;
  uint8_t minimum;
  uint8_t maximum;

  MidiAction(uint8_t astatus, uint8_t amin, uint8_t amax) :
    status(astatus), minimum(amin), maximum(amax) {}

  float scaleToFloat(uint8_t value){
    return (value-minimum)*MAX_DATA_VALUE/(maximum-minimum);
  }

  uint8_t scaleTo7(float value){
    return (uint8_t)(value*(maximum-minimum)/MAX_DATA_VALUE)+minimum;
  }
  void sendMessage(uint8_t data1, uint8_t data2){
    serialWrite(status);
    serialWrite(data1 & 0x7f);
    serialWrite(data2 & 0x7f);
  }
};

class MidiControllerAction : public MidiAction {
public:
  uint8_t data1;
  uint8_t data2;
//   MidiControllerAction() : data2(min) {}
  MidiControllerAction(uint8_t astatus, uint8_t amin, uint8_t amax, uint8_t d1, uint8_t d2) : 
    MidiAction(astatus, amin, amax), data1(d1), data2(d2) {}

  void setValue(float data){
    uint8_t value = scaleTo7(data);
    if(data2 != value){
      data2 = value;
      sendMessage(data1, data2);
    }
  }
  float getValue(){
    return scaleToFloat(data2);
  }
/*   virtual void on(float data){ */
/*     uint8_t value = scale7(data); */
/*     if(data2 != value){ */
/*       data2 = value; */
/*       sendMessage(data1, data2); */
/*     } */
/*   } */
/*   virtual void off(){ */
/*     if(data2 != minimum){ */
/*       data2 = minimum; */
/*       sendMessage(data1, data2); */
/*     } */
/*   } */
/*   virtual bool isOn(){ */
/*     return data2 != minimum; */
/*   } */
};

#endif /* _ACTION_H_ */
