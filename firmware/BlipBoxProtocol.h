#ifndef _BLIPBOXPROTOCOL_H_
#define _BLIPBOXPROTOCOL_H_

#include <inttypes.h>
#include "defs.h"
#include "Protocol.h"

class BlipBoxProtocol : public Protocol {
public:
  BlipBoxProtocol(){}
/*   ~BlipBoxProtocol(){} */
/*  data sizes without destructors: 6640, one destructor: 6700 (+60bytes), two destructors: 6712 (+12 bytes) */
  virtual void init();
  virtual void process();
private:
  void readSensors();
  void readMessage();
};

#endif /* _BLIPBOXPROTOCOL_H_ */
