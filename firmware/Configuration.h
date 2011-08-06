#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include <inttypes.h>

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
  uint8_t nrpn_parameter_msb;

  void init();
  void reset();
  void read();
  void write();
};


#endif /* _CONFIGURATION_H_ */
