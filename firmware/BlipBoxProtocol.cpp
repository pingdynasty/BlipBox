#include "BlipBoxProtocol.h"

#include <wiring.h>
// #include <avr/eeprom.h>

#include "defs.h"
#include "globals.h"
#include "Greeting.h"
#include "MessageSender.h"
#include "MessageReceiver.h"

// pins
#define POT_PIN 5
#define BUTTON1_PIN 2

MessageSender sender;
MessageReceiver receiver;

unsigned long previousMillis = 0;        // will store last time write was done

#define NO_FOLLOW_MODE 0x00
#define DOT_MODE       0x01
#define CROSS_MODE     0x02
#define CRISS_MODE     0x03
#define STAR_MODE      0x04
#define BLOB_MODE      0x05
#define SQUARE_MODE    0x06

uint8_t follow = CROSS_MODE;
uint8_t holding;
uint8_t holdCol, holdRow;
uint16_t sensitivity = SENSITIVITY; // todo: read/write to eeprom

void BlipBoxProtocol::init(){
  keys.init();
  leds.init();
  sender.init();

  // greeting
  bounce(leds);
  explode(leds);

}

void BlipBoxProtocol::process(){
  uint8_t col, row;

  if(keys.keyscan()){
    // key position has changed
    if(follow) // if follow mode = none, then the leds aren't cleared so that leds stay lit
      leds.clear();
    if(holding)
      leds.setLed(holdCol, holdRow, holding);
    if(keys.isPressed()){
      // note: inversed and swapped values!
      col = 7 - keys.getRow();
      row = 9 - keys.getColumn();
      switch(follow){
      case CROSS_MODE:
        leds.setCross(row, col, BRIGHTNESS);
        break;
      case CRISS_MODE:
        leds.setDiagonalCross(row, col, BRIGHTNESS);
        break;
      case DOT_MODE:
        leds.setLed(row, col, BRIGHTNESS);
        break;
      case STAR_MODE:
        leds.setStar(row, col, BRIGHTNESS);
        break;
      case BLOB_MODE:
        leds.setBlob(row, col, BRIGHTNESS);
        break;
      case SQUARE_MODE:
        leds.setSquare(row, col, BRIGHTNESS);
        break;
      }
    }
  }

  if(keys.getTouch() < sensitivity){
    // inverse X value
    sender.updateXY(SENSOR_MAX - keys.getX(), keys.getY(), keys.getTouch());
  }else{
    sender.updateRelease();
  }

  this->readSensors();

  if(millis() - previousMillis > SERIAL_WRITE_INTERVAL){
    sender.sendNextMessage();
    previousMillis = millis();   // remember the last time we did this
  }

  if(receiver.receiveMessage())
    this->readMessage();
}

void BlipBoxProtocol::readSensors(){

  uint16_t value;
  value = analogRead(POT_PIN);
  value &= (0xff << 2); // low level filter... todo: put a 10nF cap to ground at pot
  sender.updateSensor(POT_SENSOR, value);

  value = digitalRead(BUTTON1_PIN) == HIGH ? SENSOR_MAX : 0;

//   sender.updateSensor(BUTTON1_SENSOR, value);
//   if(sender.updateSensor(BUTTON1_SENSOR, value) && value){
//     // button has just been pressed
//     if(keys.isPressed()){
//       holding = BRIGHTNESS >> 2;
//       holdCol = 9 - keys.getColumn();
//       holdRow = 7 - keys.getRow();
//     }else{
//       holding = 0;
//     }
//     if(follow == CROSS_MODE){
//       follow = CRISS_MODE;
//     }else if(follow == CRISS_MODE){
//       follow = CROSS_MODE;
//     }
//     leds.clear(); // otherwise the hold point gets stuck
//   }

}

void BlipBoxProtocol::readMessage(){
  switch(receiver.getMessageType()){
  case CLEAR_MESSAGE:
    leds.clear();
    break;
  case FOLLOW_MODE_MESSAGE:
    follow = receiver.getMessageData()[0] & 0x0f;
    break;
  case SET_SENSITIVITY_MESSAGE:
    sensitivity = receiver.getTwoByteValue();
    break;
  case SET_LED_MESSAGE:
    if(follow){
      holding = receiver.getMessageData()[2];
      holdRow = receiver.getMessageData()[1] / COLS;
      holdCol = receiver.getMessageData()[1] % COLS;
    }else{
      leds.setLed(receiver.getMessageData()[1] / COLS, 
                  receiver.getMessageData()[1] % COLS, 
                  receiver.getMessageData()[2]);
    }
    break;
//       // 1: blipbox mode
//       // 2: monome series
//       setmode(messagedata[1]);
//       // note: does not reset, simply carries on as normal
//     }
//     break;
//   case 0x0300:
//     leds.setLed(messagedata[1] / COLS, messagedata[1] % COLS, messagedata[2]);
//     break;
//   default:
//     error(MESSAGE_READ_ERROR);
//     serialFlush();
  }
//   leds.setDiagonalCross(messagedata[0] / COLS, messagedata[0] % COLS, messagedata[1]);
//   leds.setCross(messagedata[0] / COLS, messagedata[0] % COLS, messagedata[1]);
//   leds.setLed(messagedata[0] / COLS, messagedata[0] % COLS, messagedata[1]);
}
