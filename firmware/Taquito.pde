// make TARGET=Taquito F_CPU=8000000 MCU=atmega8 UPLOAD_RATE=19200 CPP_SRC=MessageSender.cpp clean all
// make TARGET=Taquito CPP_SRC=MessageSender.cpp clean all

#include "defs.h"
#include "MessageSender.h"
// #include "MessageReceiver.h"

#define POT_PIN 0
#define MPX_PIN 1
#define BUTTON2_PIN 4
#define BUTTON3_PIN 7
#define LED_PIN 5 // digital pin 5 is not PWM capable on ATmega8! 9/10/11

#define POT_THRESH 40
#define MPX_THRESH 270 // seems to oscillate strictly from 264-266 when resting

// MPX5010DP pressure sensor
// pins:
// pin 1 Vout
// pin 2 GND
// pin 3 Vcc

uint16_t values[4];

MessageSender sender;
// MessageReceiver receiver;

unsigned long previousMillis = 0;        // will store last time write was done

void setup() {
  pinMode(BUTTON2_PIN, INPUT);
  pinMode(BUTTON3_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT); // not required for PWM output
  sender.init();

  beginSerial(DEFAULT_SERIAL_SPEED);
}

void readSensors() {
  uint16_t value, pot;

  // read buttons
  value = digitalRead(BUTTON2_PIN) == HIGH ? SENSOR_MAX : 0;
  sender.updateSensor(BUTTON2_SENSOR, value);

  value = digitalRead(BUTTON3_PIN) == HIGH ? SENSOR_MAX : 0;
  sender.updateSensor(BUTTON3_SENSOR, value);

  // read softpot (pitch control)
  pot = analogRead(POT_PIN);
  if(pot < POT_THRESH)
    pot = 0;
  sender.updateSensor(POT1_SENSOR, pot);

  // read pressure sensor (breath control)
  value = analogRead(MPX_PIN);
  if(value < MPX_THRESH){
    sender.updateRelease();
    digitalWrite(LED_PIN, LOW);
//     analogWrite(LED_PIN, 0);
  }else{
    sender.updateXY(pot, value, 0);
    digitalWrite(LED_PIN, HIGH);
    // light up the LED using pulse width modulation - scale 270-840 to 0-255
//     analogWrite(LED_PIN, ((value - MPX_THRESH) * 43) / 100);
  }
}

// void readMessage(){
//   switch(receiver.getMessageType()){
//   case SET_SENSITIVITY_MESSAGE:
//     sensitivity = receiver.getTwoByteValue();
    // todo!
//     break;
//   case CLEAR_MESSAGE:
//   case FOLLOW_MODE_MESSAGE:
//   case SET_LED_MESSAGE:
//   default:
//   }
// }

void loop() {

  readSensors();

  if(millis() - previousMillis > SERIAL_WRITE_INTERVAL){
    sender.sendNextMessage();
    previousMillis = millis();   // remember the last time we did this
  }

//   if(receiver.receiveMessage())
//     readMessage();
}
