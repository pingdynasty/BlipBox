#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_

#include <inttypes.h>
#include "globals.h"
extern LedController leds;
extern KeyController keys;

class Animator {
public:
  virtual void tick(uint8_t counter);
};

class FadeAnimator : public Animator {
public:
  FadeAnimator(uint8_t _prescaler)
    : prescaler(_prescaler) {
  }
  virtual void tick(uint8_t counter){
    if(counter % prescaler == 0)
      leds.fade(1);
  }
private:
  uint8_t prescaler;
};

class SignalAnimator : public Animator {
  /* Animator used to signal errors by turning an LED on/off */
 public:
  virtual void tick(uint8_t counter){
    if(signals){
      if(counter % 25 == 0){
        leds.setLed(0, 0, 0xff);
      }else if(counter % 25 == 12){
        leds.setLed(0, 0, 0);
        --signals;
      }
    }
  }
  void setSignal(int code){
    signals = code;
  }
 private:
  uint8_t signals;
};

#endif /* _ANIMATOR_H_ */
