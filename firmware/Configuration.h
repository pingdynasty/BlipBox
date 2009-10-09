#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include <avr/eeprom.h>
#include <inttypes.h>

#define CHECKSUM 158

class Configuration {
 public:
  uint8_t checksum;
  uint16_t serialSpeed;
  uint8_t brightness;
  uint16_t sensitivity;

  void init(){
    if(eeprom_read_byte(0) == CHECKSUM){
      this->read();
    }else{
      checksum = CHECKSUM;
      serialSpeed = DEFAULT_SERIAL_SPEED;
      brightness = BRIGHTNESS;
      sensitivity = SENSITIVITY;
    }
  }
  
  void write(){
    uint8_t* p = (uint8_t*)(void*)this;
    for(uint8_t i = 0; i < sizeof(*this); i++)
      eeprom_write_byte((uint8_t*)i, *p++);
  }

  void read(){
    uint8_t* p = (uint8_t*)(void*)this;
    for(uint8_t i = 0; i < sizeof(*this); i++)
      *p++ = eeprom_read_byte((uint8_t*)i);
  }
};


#endif /* _CONFIGURATION_H_ */
