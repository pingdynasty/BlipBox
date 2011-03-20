#ifndef _BLIPBOX_H_
#define _BLIPBOX_H_

#include "LedController.h"
#include "DisplayManager.h"
#include "KeyController.h"
#include "Animator.h"
#include "Configuration.h"
#include "MessageDispatcher.h"

// error codes
// the code corresponds to the number of blinks emmitted to signal the error
#define MESSAGE_READ_ERROR  2
#define MESSAGE_WRITE_ERROR 4

class BlipBox {
public:
  LedController leds;
  DisplayManager display;
  KeyController keys;
  SignalAnimator signal;
  Configuration config;
  MessageDispatcher sender;
  void init();
  void tick();
  void error(uint8_t code);
  void setFollowMode(uint8_t mode);
  void sendConfigurationParameters();
};

#endif /* _BLIPBOX_H_ */
