#include <avr/eeprom.h>
#include <inttypes.h>
#include "Parameters.h"

void setParameter(uint16_t data){
  switch((uint8_t)(data >> 8) & PARAMETER_ID_MASK){
  case SENSITIVITY_PARAMETER_ID:
    config.sensitivity = data & PARAMETER_VALUE_MASK;
    break;
  case BRIGHTNESS_PARAMETER_ID:
    config.brightness = data & PARAMETER_VALUE_MASK;
    break;
  case TLC_GSCLK_PERIOD_PARAMETER_ID:
    config.tlc_gsclk_period = data & PARAMETER_VALUE_MASK;
    break;
  case SERIAL_SPEED_PARAMETER_ID:
    config.serialSpeed = data & PARAMETER_VALUE_MASK;
    break;
  case FOLLOW_MODE_PARAMETER_ID:
    config.followMode = (int)(data & PARAMETER_VALUE_MASK);
    //     }else{
    //       error(MESSAGE_READ_ERROR);
    //       serialFlush();
  }
}

uint16_t getParameter(uint8_t pid){
  uint16_t data = pid << 8;
  switch(pid){
  case SENSITIVITY_PARAMETER_ID:
    data |= config.sensitivity;
    break;
  case BRIGHTNESS_PARAMETER_ID:
    data |= config.brightness;
    break;
  case TLC_GSCLK_PERIOD_PARAMETER_ID:
    data |= config.tlc_gsclk_period;
    break;
  case SERIAL_SPEED_PARAMETER_ID:
    data |= config.serialSpeed;
    break;
  case FOLLOW_MODE_PARAMETER_ID:
    data |= config.followMode;
    break;
  }
  return data;
}

// #define PID_LSB(pid) ((pid >> 2) * 2)
// #define PID_MSB(pid) (((pid >> 2) * 2) + 1)

// #define EEPROM_READ(pid) eeprom_read_byte((unsigned char*)pid)
// #define EEPROM_WRITE(pid, value) eeprom_write_byte((unsigned char*)pid, value)

// void setParameter(uint8_t pid, uint16_t value){
//   EEPROM_WRITE(PID_LSB(pid), value >> 8);
//   EEPROM_WRITE(PID_MSB(pid), value & 0xff);
// }

// uint16_t getParameter(uint8_t pid){
//   return (EEPROM_READ(PID_LSB(pid)) << 8) | EEPROM_READ(PID_MSB(pid));
// }
