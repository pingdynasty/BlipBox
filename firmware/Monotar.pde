// make TARGET=Monotar CPP_SRC="MessageSender.cpp FourWireTouchController" clean all

#include "defs.h"
#include "MessageSender.h"
#include "KeyController.h"
// #include "MessageReceiver.h"

#define TOUCH_THRESH 45
// low level reading: 22-42 when pressed; around 56-60 when at rest.

MessageSender sender;
KeyController keys;

unsigned long previousMillis = 0;        // will store last time write was done

void setup() {
  keys.init();
  sender.init();
  beginSerial(DEFAULT_SERIAL_SPEED);
}

void loop() {
  if(keys.check() < TOUCH_THRESH){
    sender.updateXY(keys.getX(), keys.getY(), keys.getZ());
    sender.updateSensor(POT1_SENSOR, keys.getZ());
  }else{
    sender.updateRelease();
  }
  // softpot value
  sender.updateSensor(POT2_SENSOR, keys.getValue(6));

  if(millis() - previousMillis > SERIAL_WRITE_INTERVAL){
    sender.sendNextMessage();
    previousMillis = millis();   // remember the last time we did this
  }
}
