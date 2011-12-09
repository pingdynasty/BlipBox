#include "ControlVoltageAction.h"
#include "spi.h"
#include "macros.h"

ControlVoltageAction::ControlVoltageAction()
  : AbstractAction(CONTROL_VOLTAGE_ACTION_TYPE), cc(0), data(0) {
  spi_init();
}

// ch	cs  ab
// 0	0   0
// 1	1   0
// 2	0   1
// 3	1   1

void ControlVoltageAction::on(float f){
  int16_t value = scaleTo14(f) >> 2;
  if(data != value){
    data = value;
    // send value to dac
    // first bit of status is 0 for channels 0 and 2, 1 for channels 1 and 3
    spi_cs(status & 0x01); 
    uint8_t flags = DAC_SHDN_BIT;
    // second bit of status is 0 for channels 0 and 1, 1 for channels 2 and 3
    if(status & 0x02)
      flags |= DAC_A_B_BIT;
    spi_send(flags | ((value >> 8) & 0x0f));
    spi_send(value & 0xff);
  }
}

void ControlVoltageAction::handle(MidiEvent& event){
  if(event.getChannel() == getChannel() && event.getData1() == cc)
    data = constrain((int16_t)(event.getData2() * 0x7f)) >> 2;
}

