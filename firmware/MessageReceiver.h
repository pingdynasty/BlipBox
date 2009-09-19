#ifndef _MESSAGERECEIVER_H_
#define _MESSAGERECEIVER_H_


// would be nice if there were 3 one-byte messages with id's in the first two bits (00xx)
// then two-byte messages with id's 01xx, three byte id's 1xxx

// marker types:
// dot, square, star, cross, criss, horizontal line, vertical line, diag line tl-br / bl-tr

// messages to blipbox, first 4 bits is message type:

// 1 byte messages
// clear : 00010000 / 0x10
// follow mode: 4 bits marker type : 0010tttt
// display effect: 4 bits effect type
// shift leds: 2 bits direction, 2 bits steps (0: up; 1: down; 2: left; 3: right)

// 2 byte messages
// configure sensors: bitmask setting sensors on/off - 12 bit sensor mask
// configure fade time: 12 bits fade time or speed
// write character message: 4 bits position, 8 bits character (fits in 7 or 6 bits?)
// set parameter message: 1000ppvv vvvvvvvv (p: parameter ID, v: 10 bit value)
// set 8 leds: 4 bits led offset, 8 bits values (on/off)

// 3 byte messages
// set led: 4 bits marker type, 8 bits led index, 8 bits brightness
// hold pos: same as set led when follow mode is on

// parameters:
// brightness: 0-255
// sensitivity: 10 bit value

// set led: led index/value
// set cross/criss/blob: 4 bits type, 8 bits led index, 8 bits brightness
// write letter: 4 bit direction/brightness, 8 bit position, 8 bit character code
// set led row: 4 bits row index, 16 bits values
// set led column: 4 bits column index, 16 bits values
// set functional mode: blipbox/monome/faders...

#include <inttypes.h>

#ifndef MESSAGE_RECEIVER_TEST
#include "globals.h"
#endif // MESSAGE_RECEIVER_TEST

#define CLEAR_MESSAGE             0x10

#define SET_LED_MESSAGE           0x20 // sets 1 led - 3 byte message
#define SET_LED_ROW_MESSAGE       0x30 // sets 8 leds - two byte message
#define SET_LED_COL_MESSAGE       0x40 // sets 8 leds - two byte message

#define WRITE_CHARACTER_MESSAGE   0x50
#define SHIFT_LEDS_MESSAGE        0x60
#define DISPLAY_EFFECT_MESSAGE    0x70

#define SET_PARAMETER_MESSAGE     0xc0
// set parameter: 11ppppvv vvvvvvvv : 4 bit parameter ID p, 10 bit value v

#define MESSAGE_ID_MASK           0xf0
#define MESSAGE_VALUE_MASK        0x0f

#define PARAMETER_ID_MASK         0x3c
#define PARAMETER_VALUE_MASK      0x03

#define MAX_MESSAGE_LENGTH 3

class MessageReceiver {

private:
  uint8_t messagedata[MAX_MESSAGE_LENGTH];
  uint8_t pos;

public:

  bool receiveMessage();

  uint8_t getMessageType();

  inline uint8_t* getMessageData(){
    return messagedata;
  }

  uint16_t getTwelveBitValue();

  uint16_t getTenBitValue();

  uint8_t getFourBitValue();

};


#endif /* _MESSAGERECEIVER_H_ */
