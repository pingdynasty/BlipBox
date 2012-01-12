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
  case PRESET_PARAMETER_ID:
    blipbox.loadPreset((uint8_t)value);
    break;
  case X_MIN_PARAMETER_ID:
    blipbox.config.touchscreen_x_min = value;
    break;
  case X_RANGE_PARAMETER_ID:
    blipbox.config.touchscreen_x_range = value;
    break;
  case Y_MIN_PARAMETER_ID:
    blipbox.config.touchscreen_y_min = value;
    break;
  case Y_RANGE_PARAMETER_ID:
    blipbox.config.touchscreen_y_range = value;
    break;
#ifdef BLIPBOX_CV4
  case CV1_PARAMETER_ID:
    blipbox.setControlVoltage(0, value<<2);
    break;
  case CV2_PARAMETER_ID:
    blipbox.setControlVoltage(1, value<<2);
    break;
  case CV3_PARAMETER_ID:
    blipbox.setControlVoltage(2, value<<2);
    break;
  case CV4_PARAMETER_ID:
    blipbox.setControlVoltage(3, value<<2);
    break;
#endif /* BLIPBOX_CV4 */
  case VERSION_PARAMETER_ID:
    break;
  default:
    blipbox.message(MESSAGE_READ_ERROR);
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
  case PRESET_PARAMETER_ID:
    data = blipbox.config.preset;
    break;
#ifdef BLIPBOX_CV4
  case CV1_PARAMETER_ID:
    data = blipbox.getControlVoltage(0)>>2;
    break;
  case CV2_PARAMETER_ID:
    data = blipbox.getControlVoltage(1)>>2;
    break;
  case CV3_PARAMETER_ID:
    data = blipbox.getControlVoltage(2)>>2;
    break;
  case CV4_PARAMETER_ID:
    data = blipbox.getControlVoltage(3)>>2;
    break;
#endif /* BLIPBOX_CV4 */
  case VERSION_PARAMETER_ID:
    data = BLIPBOX_VERSION_ID;
    break;
  default:
    blipbox.message(MESSAGE_WRITE_ERROR);
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
