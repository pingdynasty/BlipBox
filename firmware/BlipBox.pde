#include "defs.h"
#include "device.h"
#include "globals.h"

#include "MessageDispatcher.h"
#include "SerialProtocolReader.h"
#include "BlipBox.h"
#include "Parameters.h"

unsigned long previousMillis;        // will store last time write was done
uint16_t counter;

Animator* animator;
// #define NOF_ANIMATORS 5
// Animator animators[NOF_ANIMATORS] = {
// DotAnimator(), CrossAnimator(), 
// CrissAnimator(), ToggleAnimator(), StarAnimator() };
DotAnimator dot;
CrossAnimator cross;
CrissAnimator criss;
ToggleAnimator toggle;
StarAnimator star;

void setup() {
  blipbox.config.init();
  blipbox.init();
  blipbox.leds.start();
  blipbox.setMidiMode(false);
  blipbox.setEditMode(false);
  blipbox.message(ALERT);
}

void loop() {
  blipbox.tick();
//   readSensors();
  if(millis() - previousMillis > SERIAL_WRITE_INTERVAL){
    blipbox.sender.send();
    previousMillis = millis();   // remember the last time we did this
    // counter overflows at 65536
  }
  blipbox.signal.tick(++counter);
  if(animator != NULL)
    animator->tick(counter);
}

#define TAP_THRESHOLD 300 // ms between press and release to be considered tap
#define TAPTAP_THRESHOLD 600 // ms between taps to be considered double-tap

unsigned long lastpressed = 0xffff;
unsigned long lasttapped  = 0xffff;

// void BlipBox::setEventHandler(EventHandler* handler){
//   free(eventhandler);
//   eventhandler = handler;
// }

void BlipBox::setMidiMode(bool midi){
  if(midi){
    blipbox.sender.setMidiProtocol();
    beginSerial(31250L);
    blipbox.setFollowMode(5);
  }else{
    blipbox.sender.setSerialProtocol();
    beginSerial(blipbox.config.serialSpeed);
    blipbox.setFollowMode(0);
  }
}

void BlipBox::setEditMode(bool edit){
  blipbox.leds.fill(0x00);
  if(edit){
    eventhandler = &presetshandler;
    presetshandler.init();
    animator = &presetshandler;
  }else{
    eventhandler = &defaulthandler;
    blipbox.setFollowMode(0);
  }
}

void BlipBox::tick(){
  switch(keys.keyscan()){
  case DISENGAGED:
    break;
  case RELEASED:
    eventhandler->release(keys.getPosition());
    if(millis() - lasttapped < TAPTAP_THRESHOLD){
      eventhandler->taptap(keys.getPosition());
    }else if(millis() - lastpressed < TAP_THRESHOLD){
      eventhandler->tap(keys.getPosition());
      lasttapped = millis();
    }
    break;
  case PRESSED:
    eventhandler->press(keys.getPosition());
    lastpressed = millis();
    break;
  case DRAGGED:
  case UNCHANGED: // same column/row, possibly different x/y
    eventhandler->drag(keys.getPosition());
  }
}

void BlipBox::message(MessageType code){
  signal.setSignal(code);
}

void BlipBox::init() {
  cli(); // disable interrupts
  keys.init();
  leds.init();
  sender.init();
  setFollowMode(config.followMode);
  sei(); // enable interrupts
}

void BlipBox::setFollowMode(uint8_t mode) {
  config.followMode = mode;
  switch(config.followMode){
  case 1:
    animator = &dot;
    break;
  case 2:
    animator = &cross;
    break;
  case 3:
    animator = &criss;
    break;
  case 4:
    animator = &toggle;
    break;
  case 5:
    animator = &star;
    break;
  default:
    animator = NULL;
  }
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

// Interrupt routines 

#if defined(__AVR_ATmega168__)
SIGNAL(SIG_USART_RECV)
#else
SIGNAL(SIG_UART_RECV)
#endif
{
#if defined(__AVR_ATmega168__)
  unsigned char c = UDR0;
#else
  unsigned char c = UDR;
#endif
  serialInput(c);
}

ISR(TIMER1_OVF_vect)
{
  blipbox.leds.displayCurrentRow();
}

// ISR(TIMER1_OVF_vect){
//   leds.displayCurrentRow();
// }

// EMPTY_INTERRUPT(TIMER1_OVF_vect);
