#ifndef _TOUCHCONTROLLER_H_
#define _TOUCHCONTROLLER_H_

#include <stdint.h>

class TouchController {

private:
  uint16_t xval;
  uint16_t yval;
  uint16_t tval;

public:
  void init();
  uint16_t getX(){
/*     return xval; */
    return readX();
  }
  uint16_t getY(){
/*     return yval; */
    return readY();
  }
  uint16_t getTouch(){
/*     return tval; */
    return check();
  }
  uint16_t check();
  uint16_t readX();
  uint16_t readY();
};

#endif /* _TOUCHCONTROLLER_H_ */
