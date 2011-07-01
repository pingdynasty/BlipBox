#include "defs.h"
#include "device.h"
#include "globals.h"

#include "MessageDispatcher.h"
#include "BlipBox.h"
#include "Parameters.h"

#include "MidiZoneEventHandler.h"

unsigned long previousMillis;        // will store last time write was done
uint16_t counter;

Animator* animator;
// DotAnimator dot;
// CrossAnimator cross;
// CrissAnimator criss;
// ToggleAnimator toggle;
// StarAnimator star;

void setup() {
  blipbox.config.init();
  blipbox.init();
  blipbox.receiver = &blipbox.defaultreceiver;
  blipbox.eventhandler = &blipbox.defaulthandler;
  beginSerial(blipbox.config.serialSpeed);
  blipbox.message(ALERT);
  animator = NULL;
}

void loop() {
  if(millis() - previousMillis > SERIAL_WRITE_INTERVAL){
    blipbox.keys.keyscan(); // triggers eventhandler
    blipbox.sender.send();
    previousMillis = millis();   // remember the last time we did this
  }
  // counter overflows at 65536
  blipbox.signal.tick(++counter);
  if(animator != NULL)
    animator->tick(counter);
}

void BlipBox::setSerialReader(SerialReader* handler){
  SerialReader* p = receiver;
  receiver = handler;
  if(p != &defaultreceiver && p != receiver)
    free(p);
}

void BlipBox::resetSerialReader(){
  SerialReader* p = receiver;
  receiver = &defaultreceiver;
  if(p != &defaultreceiver)
    free(p);
}

void BlipBox::setEventHandler(EventHandler* handler){
  EventHandler* p = eventhandler;
  eventhandler = handler;
  if(p != &defaulthandler && p != &midizones && p != handler)
    free(p);
  eventhandler->init();
}

void BlipBox::resetEventHandler(){
  EventHandler* p = eventhandler;
  eventhandler = &defaulthandler;
  if(p != &defaulthandler && p != &midizones)
    free(p);
}

void BlipBox::setMidiMode(bool midi){
  if(midi){
    beginSerial(31250L);
    setSerialReader(new SerialReader());
    eventhandler = &midizones;
    animator = &midizones;
  }else{
    beginSerial(blipbox.config.serialSpeed);
//     resetSerialReader(); // done by setEditMode(false)
//     resetEventHandler(); // done by setEditMode(false)
//     blipbox.setFollowMode(0); // done by setEditMode(false)
  }
}

void BlipBox::setEditMode(bool edit){
  blipbox.leds.clear();
  if(edit){
    PresetChooser* presetshandler = new PresetChooser();
    setEventHandler(presetshandler);
    setSerialReader(new SerialReader());
    animator = presetshandler;
  }else{
    resetEventHandler();
    resetSerialReader();
    animator = NULL;
//     blipbox.setFollowMode(0);
  }
}

// #define TAP_THRESHOLD    300 // ms between press and release to be considered tap
// #define TAPTAP_THRESHOLD 600 // ms between taps to be considered double-tap
// unsigned long lastpressed   = LONG_MAX;
// unsigned long lasttapped    = LONG_MAX;
// unsigned long lastreleased  = LONG_MAX;

void BlipBox::message(MessageType code){
  signal.setSignal(code);
}

void BlipBox::init() {
  cli(); // disable interrupts
  keys.init();
  leds.init();
  sender.init();
//   setFollowMode(config.followMode);
  sei(); // enable interrupts
}

void BlipBox::setFollowMode(uint8_t mode) {
  config.followMode = mode;
//   switch(config.followMode){
//   case 1:
//     animator = &dot;
//     break;
//   case 2:
//     animator = &cross;
//     break;
//   case 3:
//     animator = &criss;
//     break;
//   case 4:
//     animator = &toggle;
//     break;
//   case 5:
//     animator = &star;
//     break;
//   default:
//     animator = NULL;
//   }
}

void sendParameter(uint8_t pid){
  blipbox.sender.parameter.update(pid, getParameter(pid));
  blipbox.sender.parameter.send();
}

void BlipBox::sendConfigurationParameters(){
  message(ALERT);
  sendParameter(SENSITIVITY_PARAMETER_ID);
  sendParameter(BRIGHTNESS_PARAMETER_ID);
  sendParameter(TLC_GSCLK_PERIOD_PARAMETER_ID);
  sendParameter(SERIAL_SPEED_PARAMETER_ID);
  sendParameter(FOLLOW_MODE_PARAMETER_ID);
}

void BlipBox::sendMidiZones(){
  blipbox.sender.parameter.update(MIDI_ZONE_PARAMETER_ID, midizones.preset);
  blipbox.sender.parameter.send();
  uint8_t data[4];
  for(int i=0; i<8; ++i){
    midizones.getZone(i).write(data);
    for(int j=0; j<4; ++j){
      blipbox.sender.parameter.update(MIDI_ZONE_PARAMETER_ID, data[j]);
      blipbox.sender.parameter.send();
    }
  }
}

// Interrupt routines 

#if defined(__AVR_ATmega168__)
SIGNAL(SIG_USART_RECV){
  unsigned char c = UDR0;
#else
SIGNAL(SIG_UART_RECV){
  unsigned char c = UDR;
#endif
  blipbox.receiver->serialInput(c);
}

ISR(TIMER1_OVF_vect){
  blipbox.leds.displayCurrentRow();
}
