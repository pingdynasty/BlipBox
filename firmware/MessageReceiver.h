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

// 2 byte messages
// configure sensors: bitmask setting sensors on/off - 12 bit sensor mask
// configure fade time: 12 bits fade time or speed
// configure sensitivity: 10 bits sensitivity value : 0011ssss ssssssss
// write character message: 4 bits position, 8 bits character (fits in 7 or 6 bits?)

// 3 byte messages
// set led: 4 bits marker type, 8 bits led index, 8 bits brightness

// hold pos: 4 bits marker type, 8 bits led index, 8 bits brightness

// set brightness: 0-255
// set sensitivity: 10 bit value
// set led: led index/value
// set cross/criss/blob: 4 bits type, 8 bits led index, 8 bits brightness
// write letter: 4 bit direction/brightness, 8 bit position, 8 bit character code
// set led row: 4 bits row index, 16 bits values
// set led column: 4 bits column index, 16 bits values
// set functional mode: blipbox/monome/faders...

#include <inttypes.h>

#ifndef MESSAGE_RECEIVER_TEST
#include <wiring.h>
#include "globals.h"
#endif // MESSAGE_RECEIVER_TEST

#define MAX_MESSAGE_LENGTH 3

#define CLEAR_MESSAGE             0x10
#define FOLLOW_MODE_MESSAGE       0x20
#define SET_SENSITIVITY_MESSAGE   0x30
#define SET_LED_MESSAGE           0x40
#define WRITE_CHARACTER_MESSAGE   0x50
#define SHIFT_LEDS_MESSAGE        0x60

#define MESSAGE_ID_MASK           0xf0
#define MESSAGE_VALUE_MASK        0x0f

class MessageReceiver {

private:
  uint8_t messagedata[MAX_MESSAGE_LENGTH];
  uint8_t pos;

public:

  bool receiveMessage(){
    // one byte is read per iteration,
    // and true is only returned once a full message is read.
    // otherwise return false to read another byte on the next call
    if(serialAvailable()){
      messagedata[pos++] = serialRead();
      switch(getMessageType()){
        // 3 byte messages
      case SET_LED_MESSAGE:
        if(pos == 3)
          pos = 0;
        break;
        // 2 byte messages
      case WRITE_CHARACTER_MESSAGE:
      case SET_SENSITIVITY_MESSAGE:
        if(pos == 2)
          pos = 0;
        break;
        // 1 byte messages
      case CLEAR_MESSAGE:
      case SHIFT_LEDS_MESSAGE:
      case FOLLOW_MODE_MESSAGE:
        pos = 0;
        break;
      default:
        pos = 0;
//         serialFlush();
        error(MESSAGE_READ_ERROR);
        return false;
      }
      // pos == 0 iff a full message has been read
      return pos == 0;
    }
    return false;
  }

  uint8_t getMessageType(){
    return messagedata[0] & MESSAGE_ID_MASK;
  }

  uint16_t getTwoByteValue(){
    return ((messagedata[0] & MESSAGE_VALUE_MASK) << 8) | messagedata[1];
  }

  uint8_t* getMessageData(){
    return messagedata;
  }

};


#endif /* _MESSAGERECEIVER_H_ */
