#ifndef _MONOMESERIESPROTOCOL_H_
#define _MONOMESERIESPROTOCOL_H_

#include <inttypes.h>
#include "Protocol.h"
#include "defs.h"

#ifndef MAX_MESSAGE_LENGTH
#define MAX_MESSAGE_LENGTH 5
#endif

//#define MSP_ARG_TYPE const uint8_t& // using const uint8_t& leads to 464 bytes larger binary
#define MSP_ARG_TYPE uint8_t

class MonomeSeriesProtocol : public Protocol {
public:
  MonomeSeriesProtocol()
   : brightness(0xff)
    {}
  virtual void init();
  virtual void process();
  void readMessage();
  void keyUp(uint8_t x, uint8_t y);
  void keyDown(uint8_t x, uint8_t y);
private:
  uint8_t brightness;
  uint8_t data[MAX_MESSAGE_LENGTH];

  void led_on(MSP_ARG_TYPE row, MSP_ARG_TYPE col);
  void led_off(MSP_ARG_TYPE row, MSP_ARG_TYPE col);

  void led_row1(MSP_ARG_TYPE row, MSP_ARG_TYPE col);
  void led_col1(MSP_ARG_TYPE row, MSP_ARG_TYPE col);
  void led_row2(MSP_ARG_TYPE row, MSP_ARG_TYPE rowa, MSP_ARG_TYPE rowb);
  void led_col2(MSP_ARG_TYPE row, MSP_ARG_TYPE rowa, MSP_ARG_TYPE rowb);
  void led_frame(MSP_ARG_TYPE quadrant, uint8_t* rows);
  void clear(MSP_ARG_TYPE state);
  void mode(MSP_ARG_TYPE state);
  void intensity(MSP_ARG_TYPE level);
  void port_on(MSP_ARG_TYPE level);
  void port_off(MSP_ARG_TYPE level);
};

#endif /* _MONOMESERIESPROTOCOL_H_ */
