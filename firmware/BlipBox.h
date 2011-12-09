#ifndef _BLIPBOX_H_
#define _BLIPBOX_H_

#include "EventHandler.h"
#include "PresetChooser.h"
#include "LedController.h"
#include "DisplayManager.h"
#include "KeyController.h"
#include "Animator.h"
#include "Configuration.h"
#include "SerialReader.h"
#include "Preset.h"

// #define nullptr NULL
// #define noexcept
// #define jassert(expression)      {}
// #include "juce_ScopedPointer.h"

// error codes
// the code corresponds to the number of blinks emmitted to signal the error
enum MessageType {
  ALERT = 2, MESSAGE_READ_ERROR = 4 , MESSAGE_WRITE_ERROR = 6
};

void setup();
void loop();

class BlipBox {
public:
  LedController leds;
  DisplayManager display;
  KeyController keys;
  SignalAnimator signal;
  Configuration config;
  Preset preset;
//   ScopedPointer<SerialReader> receiver;
//   ScopedPointer<EventHandler> eventhandler;
//   ScopedPointer<Animator> animator;
  SerialReader* receiver;
  EventHandler* eventhandler;
  Animator* animator;
  void init();
  void message(MessageType code);
  void sendParameter(uint8_t pid);
  void sendConfigurationParameters();
  void sendPreset();
  void setSerialReader(SerialReader* handler);
  void resetSerialReader();
  void setEventHandler(EventHandler* handler);
  void resetEventHandler();
  void setEditMode(bool edit);
  void setMidiMode(bool midi);
  void loadPreset(uint8_t preset);
  void loop();
private:
  unsigned long previousMillis;        // will store last time write was done
  uint16_t counter;
};

#endif /* _BLIPBOX_H_ */
