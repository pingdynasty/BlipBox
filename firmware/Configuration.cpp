#include "Configuration.h"
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
#define DEFAULT_NRPN_MSB     3

void Configuration::init(){
  checksum = sizeof(*this);
  if(eeprom_read_byte(CONFIGURATION_EEPROM_OFFSET) == checksum)
    read();
  else
    reset();
}

void Configuration::reset(){
  brightness          = DEFAULT_BRIGHTNESS;
  sensitivity         = DEFAULT_SENSITIVITY;
  tlc_gsclk_period    = DEFAULT_GSCLK_PERIOD;
  serialSpeed         = DEFAULT_SERIAL_SPEED;
  touchscreen_x_min   = DEFAULT_X_MIN;
  touchscreen_x_range = DEFAULT_X_RANGE;
  touchscreen_y_min   = DEFAULT_Y_MIN;
  touchscreen_y_range = DEFAULT_Y_RANGE;
  nrpn_parameter_msb  = DEFAULT_NRPN_MSB;
}

void Configuration::read(){
  eeprom_read_block((uint8_t*)this, CONFIGURATION_EEPROM_OFFSET, sizeof(this));
}

void Configuration::write(){
  eeprom_write_block((uint8_t*)this, CONFIGURATION_EEPROM_OFFSET, sizeof(this));
}



// uint8_t config_brightness EEMEM = DEFAULT_BRIGHTNESS;
// uint16_t config_sensitivity EEMEM = DEFAULT_SENSITIVITY;
// uint8_t config_tlc_gsclk_period EEMEM = DEFAULT_GSCLK_PERIOD;
// uint16_t config_serialSpeed EEMEM = DEFAULT_SERIAL_SPEED;
// uint8_t config_followMode EEMEM = DEFAULT_FOLLOW_MODE;
// uint16_t config_touchscreen_x_min EEMEM = DEFAULT_X_MIN;
// uint16_t config_touchscreen_x_range EEMEM = DEFAULT_X_RANGE;
// uint16_t config_touchscreen_y_min EEMEM = DEFAULT_Y_MIN;
// uint16_t config_touchscreen_y_range EEMEM = DEFAULT_Y_RANGE;

// uint8_t eepromcfg[sizeof(Configuration)] EEMEM = {
//   DEFAULT_BRIGHTNESS,
//   DEFAULT_SENSITIVITY,
//   DEFAULT_FOLLOW_MODE,
//   TLC_GSCLK_PERIOD,
//   DEFAULT_SERIAL_SPEED,
//   DEFAULT_X_MIN,
//   DEFAULT_X_RANGE,
//   DEFAULT_Y_MIN,
//   DEFAULT_Y_RANGE};

// Configuration eepromcfg EEMEM  = 
//   Configuration(sizeof(Configuration),
// 		DEFAULT_BRIGHTNESS,
// 		DEFAULT_SENSITIVITY,
// 		DEFAULT_FOLLOW_MODE,
// 		TLC_GSCLK_PERIOD,
// 		DEFAULT_SERIAL_SPEED,
// 		DEFAULT_X_MIN,
// 		DEFAULT_X_RANGE,
// 		DEFAULT_Y_MIN,
// 		DEFAULT_Y_RANGE);

// void Configuration::read(){
//   eeprom_read_block((uint8_t*)(void*)this, &eepromcfg, sizeof(this));
//     uint8_t* p = (uint8_t*)(void*)this;
//     for(uint8_t i = 0; i < sizeof(*this); i++)
//       *p++ = eeprom_read_byte((uint8_t*)i);
//   }

// void Configuration::write(){
//   eeprom_write_block((uint8_t*)(void*)this, &eepromcfg, sizeof(this));
// }

//     uint8_t* p = (uint8_t*)(void*)this;
//     for(uint8_t i = 0; i < sizeof(*this); i++)
//       eeprom_write_byte((uint8_t*)i, *p++);

