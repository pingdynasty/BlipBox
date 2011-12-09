#ifndef _CONTROL_VOLTAGE_ACTION_H_
#define _CONTROL_VOLTAGE_ACTION_H_

#include "Action.h"

class ControlVoltageAction : public AbstractAction {
  uint8_t cc;
  int16_t data;
public:
  ControlVoltageAction();
  void on(float f);
  float getValue(){ 
    return scaleToFloat((int16_t)(data << 2));
  }
  uint8_t read(const uint8_t* data){
    cc = data[6];
    return AbstractAction::read(data)+1;
  }
  uint8_t write(uint8_t* data){
    data[6] = cc;
    return AbstractAction::write(data)+1;
  }
  void handle(MidiEvent& event);
};

#endif /* _CONTROL_VOLTAGE_ACTION_H_ */
