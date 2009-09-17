#ifndef _BLIPBOXPROTOCOL_H_
#define _BLIPBOXPROTOCOL_H_

#include <inttypes.h>
#include "defs.h"
#include "Protocol.h"

class BlipBoxProtocol {
/* class BlipBoxProtocol : public Protocol { */
public:
  BlipBoxProtocol(){}
/*   ~BlipBoxProtocol(){} */
/*  data sizes without destructors: 6640, one destructor: 6700 (+60bytes), two destructors: 6712 (+12 bytes) */
 void init();
 void process();
/*   virtual void init(); */
/*   virtual void process(); */
private:
  uint8_t counter;
  void readSensors();
  void readMessage();
  void setSensitivity(uint16_t value);
  void setBrightness(uint16_t value);
};

#endif /* _BLIPBOXPROTOCOL_H_ */
