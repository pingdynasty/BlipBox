#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include <avr/eeprom.h>
#include <inttypes.h>

#define TLC_GSCLK_PERIOD 7

/* #define DEFAULT_SERIAL_SPEED 9600 */
/* #define DEFAULT_SERIAL_SPEED 19200L */
/* #define DEFAULT_SERIAL_SPEED 31250L /\* MIDI SPEED *\/ */
/* #define DEFAULT_SERIAL_SPEED 38400L */
#define DEFAULT_SERIAL_SPEED 57600L
/* #define DEFAULT_SERIAL_SPEED 115200L */
/* #define SENSITIVITY 120 // the lower the value, the less sensitive */
/* #define SENSITIVITY 600 // the lower the value, the less sensitive */
#define SENSITIVITY 200 // the lower the value, the less sensitive
#define BRIGHTNESS 0xff

class Configuration {
 public:
  uint8_t checksum;
  uint8_t brightness;
  uint16_t sensitivity;
  uint8_t tlc_gsclk_period;
  uint16_t serialSpeed;

  void init(){
    checksum = sizeof(*this);
    if(eeprom_read_byte(0) == checksum)
      this->read();
    else
      this->reset();
  }

  void reset(){
    brightness = BRIGHTNESS;
    sensitivity = SENSITIVITY;
    tlc_gsclk_period = TLC_GSCLK_PERIOD;
    serialSpeed = DEFAULT_SERIAL_SPEED;
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
