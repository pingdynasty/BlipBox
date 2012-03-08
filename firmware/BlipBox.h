#ifndef _BLIPBOX_H_
#define _BLIPBOX_H_

#include "EventHandler.h"
#include "LedController.h"
#include "DisplayManager.h"
#include "KeyController.h"
#include "Animator.h"
#include "Configuration.h"
#include "SerialReader.h"
#include "Preset.h"
#include "defs.h"

// error codes
// the code corresponds to the number of blinks emmitted to signal the error
enum MessageType {
  ALERT = 2, 
  MESSAGE_READ_ERROR = 4, 
  MESSAGE_WRITE_ERROR = 6
};

void setup();
void loop();

enum BlipBoxMode {
  EDIT_MODE = 1,
  MIDI_MODE = 2,
  BLIP_MODE = 3 // USB serial mode, BlipBox protocol
};

class BlipBox {
public:
#ifdef BLIPBOX_CV4
  uint16_t getControlVoltage(uint8_t channel);
  void setControlVoltage(uint8_t channel, uint16_t value);
  uint16_t controlvoltages[4];
#endif /* BLIPBOX_CV4 */
  LedController leds;
  DisplayManager display;
  KeyController keys;
  SignalAnimator signal;
  Configuration config;
  Preset preset;
  SerialReader* receiver;
  EventHandler* eventhandler;
  Animator* animator;
  void init();
  void message(MessageType code);
  void sendParameter(uint8_t pid);
  void sendConfigurationParameters();
  void sendPreset(uint8_t index);
  void receivePreset(uint8_t index);
  void setSerialReader(SerialReader* handler);
  void resetSerialReader();
  void setEventHandler(EventHandler* handler);
  void resetEventHandler();
  void setMode(BlipBoxMode mode);
  void loadPreset(uint8_t preset);
  uint8_t getPresetIndex();
  void loop();
private:
  unsigned long previousMillis;        // will store last time loop was done
  uint16_t counter;
};

#endif /* _BLIPBOX_H_ */
