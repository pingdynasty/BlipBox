#include <avr/eeprom.h>
#include <inttypes.h>
#include "Parameters.h"

void setParameter(uint8_t pid, uint16_t value){
  switch(pid){
  case SENSITIVITY_PARAMETER_ID:
    blipbox.config.sensitivity = value;
    break;
  case BRIGHTNESS_PARAMETER_ID:
    blipbox.config.brightness = value;
    break;
  case TLC_GSCLK_PERIOD_PARAMETER_ID:
    blipbox.config.tlc_gsclk_period = value;
    break;
  case SERIAL_SPEED_PARAMETER_ID:
    blipbox.config.serialSpeed = value == 0 ?
      31250L : value * 9600;
    break;
  case FOLLOW_MODE_PARAMETER_ID:
    blipbox.setFollowMode((uint8_t)value);
    break;
  default:
    blipbox.error(MESSAGE_READ_ERROR);
  }
}

uint16_t getParameter(uint8_t pid){
  uint16_t data;
  switch(pid){
  case SENSITIVITY_PARAMETER_ID:
    data = blipbox.config.sensitivity;
    break;
  case BRIGHTNESS_PARAMETER_ID:
    data = blipbox.config.brightness;
    break;
  case TLC_GSCLK_PERIOD_PARAMETER_ID:
    data = blipbox.config.tlc_gsclk_period;
    break;
  case SERIAL_SPEED_PARAMETER_ID:
    data = blipbox.config.serialSpeed == 0 ?
      31250L : blipbox.config.serialSpeed / 9600;
    break;
  case FOLLOW_MODE_PARAMETER_ID:
    data = blipbox.config.followMode;
    break;
  default:
    blipbox.error(MESSAGE_WRITE_ERROR);
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
