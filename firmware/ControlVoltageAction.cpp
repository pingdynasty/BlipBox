#include "ControlVoltageAction.h"
#include "globals.h"

ControlVoltageAction::ControlVoltageAction()
  : MidiAction(CONTROL_VOLTAGE_ACTION_TYPE), cc(0) {
}

float ControlVoltageAction::getValue(){ 
  return scaleFrom14(blipbox.getControlVoltage(getChannel()) << 2);
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

