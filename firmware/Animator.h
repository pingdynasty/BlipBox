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
      if(counter % 25 < 12){
        leds.setLed(0, 0, 0xff);
      }else if(counter % 25 == 24){
        --signals;
      }else{
        leds.setLed(0, 0, 0);
      }
    }
  }
  void setSignal(int code){
    signals = code;
  }
 private:
  uint8_t signals;
};

/* class CrossAnimator : public Animator { */
/*  public: */
/*   virtual void tick(uint8_t counter){ */
/*     leds.fade(1); */
/*     if(keys.isPressed()){ */
/*       leds.setLed(9 - keys.getColumn(), 7 - keys.getRow()); */
/*       // and then do cross */
/*     } */
/*   } */
/* }; */

// class TextAnimator : public Animator {
// public:
//   virtual void tick(uint8_t counter){
//   }
// private:
//   char* text;
//   uint8_t prescaler
// };

#endif /* _ANIMATOR_H_ */
