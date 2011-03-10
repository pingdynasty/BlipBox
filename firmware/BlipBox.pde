#include "defs.h"
#include "device.h"
#include "globals.h"
#include "MessageSender.h"
#include "SerialProtocolReader.h"

unsigned long previousMillis = 0;        // will store last time write was done
Animator* animator;
uint8_t counter;

DotAnimator dot;

void setup() {

  cli(); // disable interrupts

//   disable_watchdog(); // disable watchdog timer
  // wdt_init causes device to hang? setup gets stuck?

  config.init();
  keys.init();
  leds.init();
  sender.init();

  sei(); // enable interrupts

  beginSerial(config.serialSpeed);

  animator = NULL;
//   animator = &dot;

  leds.start();
}

void loop() {
//   if(keys.getZ() < config.sensitivity){
  if(keys.keyscan()){
    sender.updateXY(keys.getX(), keys.getY(), keys.getZ());
  }else{
    sender.updateRelease();
  }

//   readSensors();

  if(millis() - previousMillis > SERIAL_WRITE_INTERVAL){
    sender.sendNextMessage();
    previousMillis = millis();   // remember the last time we did this

    // counter overflows automatically from 255 back to 0
    signal.tick(counter++);
    if(animator)
      animator->tick(counter);

    switch(config.followMode){
    case 1:
      dot.tick(counter);
      break;
    }
  }
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
  leds.displayCurrentRow();
}

// ISR(TIMER1_OVF_vect){
//   leds.displayCurrentRow();
// }

// EMPTY_INTERRUPT(TIMER1_OVF_vect);
