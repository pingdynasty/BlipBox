#include "defs.h"
#include "device.h"
#include "globals.h"

#include "MessageDispatcher.h"
#include "SerialProtocolReader.h"
#include "BlipBox.h"
#include "Parameters.h"

unsigned long previousMillis;        // will store last time write was done
uint8_t counter;

Animator* animator;
DotAnimator dot;
CrossAnimator cross;
CrissAnimator criss;
ToggleAnimator toggle;

void setup() {
//   disable_watchdog(); // disable watchdog timer
// wdt_init causes device to hang? setup gets stuck?
  blipbox.config.init();
  blipbox.init();
  blipbox.leds.start();
  blipbox.error(MESSAGE_WRITE_ERROR);
}

void loop() {
  blipbox.tick();
//   readSensors();
  if(millis() - previousMillis > SERIAL_WRITE_INTERVAL){
    blipbox.sender.send();
    previousMillis = millis();   // remember the last time we did this
    // counter overflows at 255
    blipbox.signal.tick(counter++);
    if(animator != NULL)
      animator->tick(counter);
  }
}

void BlipBox::tick(){
  switch(keys.keyscan()){
  case RELEASED:
    sender.release.update();
    sender.position.reset();
    break;
  case DISENGAGED:
    break;
  default:
    sender.position.update(keys.getX(), keys.getY());
  }
}

void BlipBox::error(uint8_t code){
  signal.setSignal(code);
}

void BlipBox::init() {
  cli(); // disable interrupts
  keys.init();
  leds.init();
  sender.init();
  beginSerial(config.serialSpeed);
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
  default:
    animator = NULL;
  }
}

void sendParameter(uint8_t pid){
  blipbox.sender.parameter.update(pid, getParameter(pid));
  blipbox.sender.parameter.send();
}

void BlipBox::sendConfigurationParameters(){
  error(MESSAGE_WRITE_ERROR);
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
