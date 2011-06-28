#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include <avr/eeprom.h>
#include <inttypes.h>

#define CONFIGURATION_EEPROM_OFFSET 0

#define DEFAULT_GSCLK_PERIOD 7
#define DEFAULT_SERIAL_SPEED 57600L
#define DEFAULT_SENSITIVITY  80   // the higher the value, the more sensitive
#define DEFAULT_BRIGHTNESS   0xff
#define DEFAULT_FOLLOW_MODE  0x00
#define DEFAULT_X_MIN        220
#define DEFAULT_X_RANGE      760 - DEFAULT_X_MIN
#define DEFAULT_Y_MIN        215
#define DEFAULT_Y_RANGE      830 - DEFAULT_Y_MIN

class Configuration {
 public:
  uint8_t checksum;
  uint8_t brightness;
  uint16_t sensitivity;
  uint8_t tlc_gsclk_period;
  uint16_t serialSpeed;
  uint8_t followMode;
  uint16_t touchscreen_x_min;
  uint16_t touchscreen_x_range;
  uint16_t touchscreen_y_min;
  uint16_t touchscreen_y_range;

  void init(){
    read();
    checksum = sizeof(*this);
    if(eeprom_read_byte(0) == checksum)
      this->read();
    else
      this->reset();
  }

  void reset(){
    brightness          = DEFAULT_BRIGHTNESS;
    sensitivity         = DEFAULT_SENSITIVITY;
    tlc_gsclk_period    = DEFAULT_GSCLK_PERIOD;
    serialSpeed         = DEFAULT_SERIAL_SPEED;
    touchscreen_x_min   = DEFAULT_X_MIN;
    touchscreen_x_range = DEFAULT_X_RANGE;
    touchscreen_y_min   = DEFAULT_Y_MIN;
    touchscreen_y_range = DEFAULT_Y_RANGE;
  }

  void read(){
    eeprom_read_block((uint8_t*)this, CONFIGURATION_EEPROM_OFFSET, sizeof(this));
//     uint8_t* p = (uint8_t*)(void*)this;
//     for(uint8_t i = 0; i < sizeof(*this); i++)
//       *p++ = eeprom_read_byte((uint8_t*)i);
  }

  void write(){
    eeprom_write_block((uint8_t*)this, CONFIGURATION_EEPROM_OFFSET, sizeof(this));
//     uint8_t* p = (uint8_t*)(void*)this;
//     for(uint8_t i = 0; i < sizeof(*this); i++)
//       eeprom_write_byte((uint8_t*)i, *p++);
  }

};


#endif /* _CONFIGURATION_H_ */
