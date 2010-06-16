/* interval at which to write sensor output (milliseconds) */
#define SERIAL_WRITE_INTERVAL 20L
#define DEFAULT_SERIAL_SPEED 57600L

#include "MessageSender.h"
// #include "MessageReceiver.h"
#include "adc_freerunner.h"

#define SENSOR_MAX          1023

#define AMP_PIN 5
#define MPX_PIN 4
#define POT_PIN 3
#define FSR_PIN 2

#define MPX_THRESH 40

// MPX5010DP pressure sensor
// pins:
// pin 1 Vout
// pin 2 GND
// pin 3 Vcc

uint16_t values[4];

MessageSender sender;

unsigned long previousMillis = 0;        // will store last time write was done

void setup() {
  sender.init();
  setup_adc();
  beginSerial(DEFAULT_SERIAL_SPEED);
}

uint16_t breath, pressure, pot;

void readSensors() {

  pot = getAnalogValue(POT_PIN);

//   sender.updateSensor(POT1_SENSOR, getAnalogValue(POT_PIN));

  pressure = getAnalogValue(FSR_PIN);

  // read pressure sensor (breath control)
  breath = getAnalogValue(MPX_PIN);
  if(breath < MPX_THRESH){
    sender.updateRelease();
  }else{
    breath = getAnalogValue(AMP_PIN);
    sender.updateXY(pot, breath, 0); // can't use real Z values yet
  }
  sender.updateSensor(POT1_SENSOR, pressure);
}

void loop() {

  readSensors();

  if(millis() - previousMillis > SERIAL_WRITE_INTERVAL){
    sender.sendNextMessage();
    previousMillis = millis();   // remember the last time we did this
  }

//   if(receiver.receiveMessage())
//     readMessage();
}


