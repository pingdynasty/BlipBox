#include "Action.h"
#include "spi.h"

ControlVoltageAction::ControlVoltageAction()
  : AbstractAction(CONTROL_VOLTAGE_ACTION_TYPE), cc(0), data(0) {
  spi_init();
}

void ControlVoltageAction::on(float f){
  int16_t value = scaleTo14(f) >> 2;
  if(data != value){
    data = value;
    // send value to dac
    spi_cs(channel & 0x01);
    uint8_t flags = DAC_SHDN_BIT;
    if(channel & 0x02)
      flags |= DAC_A_B_BIT;
    spi_send(flags | ((value >> 8) & 0x0f));
    spi_send(value & 0x0f);
  }
}

