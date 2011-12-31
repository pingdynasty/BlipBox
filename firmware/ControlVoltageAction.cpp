#include "ControlVoltageAction.h"
#include "globals.h"

ControlVoltageAction::ControlVoltageAction()
  : AbstractAction(CONTROL_VOLTAGE_ACTION_TYPE), cc(0) {
}

// ch	cs  ab
// 0	0   0
// 1	1   0
// 2	0   1
// 3	1   1

float ControlVoltageAction::getValue(){ 
  return scaleToFloat((int16_t)(blipbox.getControlVoltage(getChannel())) << 2);
}

void ControlVoltageAction::on(float f){
  int16_t value = scaleTo14(f) >> 2;
  blipbox.setControlVoltage(getChannel(), value);
}

void ControlVoltageAction::handle(MidiEvent& event){
  if(event.getChannel() == getChannel() && event.getData1() == cc)
    blipbox.setControlVoltage(getChannel(), event.getData2() << 5);
//     blipbox.setControlVoltage(getChannel(), constrain((int16_t)(event.getData2() * 0x7f)) >> 2);
}

