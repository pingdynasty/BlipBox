#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_

#include <inttypes.h>

class Animator {
public:
  virtual void tick(uint16_t counter);
};

class FadeAnimator : public Animator {
public:
  FadeAnimator(uint8_t _prescaler)
    : prescaler(_prescaler) {
  }
  virtual void tick(uint16_t counter);
private:
  uint8_t prescaler;
};

class SignalAnimator : public Animator {
  /* Animator used to signal errors by turning an LED on/off */
 public:
  void tick(uint16_t counter);
  void setSignal(int code){
    signals = code;
  }
 private:
  uint8_t signals;
};

class DotAnimator : public Animator {
 public:
  void tick(uint16_t counter);
};

class CrossAnimator : public Animator {
 public:
  void tick(uint16_t counter);
};

class CrissAnimator : public Animator {
 public:
  void tick(uint16_t counter);
};

class StarAnimator : public Animator {
 public:
  void tick(uint16_t counter);
};

class ToggleAnimator : public Animator {
 public:
  void tick(uint16_t counter);
};

// class TextAnimator : public Animator {
// public:
//   virtual void tick(uint16_t counter){
//   }
// private:
//   char* text;
//   uint8_t prescaler
// };

#endif /* _ANIMATOR_H_ */
