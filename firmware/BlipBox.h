#ifndef _BLIPBOX_H_
#define _BLIPBOX_H_

#include "EventHandler.h"
#include "PresetChooser.h"
#include "LedController.h"
#include "DisplayManager.h"
#include "KeyController.h"
#include "Animator.h"
#include "Configuration.h"
#include "MessageDispatcher.h"
#include "SerialReader.h"
#include "SerialProtocolReader.h"
#include "MidiZoneEventHandler.h"

// error codes
// the code corresponds to the number of blinks emmitted to signal the error
/* #define SIGNAL_ERROR  2 */
/* #define MESSAGE_READ_ERROR  2 */
/* #define MESSAGE_WRITE_ERROR 4 */
enum MessageType {
  ALERT = 2, MESSAGE_READ_ERROR = 4 , MESSAGE_WRITE_ERROR = 6
};

class BlipBox {
public:
  LedController leds;
  DisplayManager display;
  KeyController keys;
  SignalAnimator signal;
  Configuration config;
  MessageDispatcher sender;
  MidiZoneEventHandler midizones;
  SerialReader* receiver;
  EventHandler* eventhandler;
  void init();
  void tick();
  void message(MessageType code);
  void setFollowMode(uint8_t mode);
  void sendConfigurationParameters();
  void setSerialReader(SerialReader* handler);
  void resetSerialReader();
  void setEventHandler(EventHandler* handler);
  void resetEventHandler();
  void setEditMode(bool edit);
  void setMidiMode(bool midi);
private:
  SerialProtocolReader defaultreceiver;
  DefaultEventHandler defaulthandler;
};

#endif /* _BLIPBOX_H_ */
