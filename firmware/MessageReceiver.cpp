#include "MessageReceiver.h"

#include <wiring.h>

// #define MAX_MESSAGE_LENGTH 3
// uint8_t messagedata[MAX_MESSAGE_LENGTH];

bool MessageReceiver::receiveMessage(){
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
    case SET_LED_ROW_MESSAGE:
    case SET_LED_COL_MESSAGE:
    case WRITE_CHARACTER_MESSAGE:
    case SET_PARAMETER_MESSAGE:
      if(pos == 2)
        pos = 0;
      break;
      // 1 byte messages
    case CLEAR_MESSAGE:
    case SHIFT_LEDS_MESSAGE:
    case DISPLAY_EFFECT_MESSAGE:
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

uint8_t MessageReceiver::getMessageType(){
  return messagedata[0] & MESSAGE_ID_MASK;
}

/* extract 12 bits of data from the first two message bytes as a 16 bit value */
uint16_t MessageReceiver::getTwelveBitValue(){
  return ((messagedata[0] & MESSAGE_VALUE_MASK) << 8) | messagedata[1];
}

/* extract 10 bits of data from the first two message bytes as a 16 bit value */
uint16_t MessageReceiver::getTenBitValue(){
  return ((messagedata[0] & PARAMETER_VALUE_MASK) << 8) | messagedata[1];
}

uint8_t MessageReceiver::getFourBitValue(){
  return messagedata[0] & MESSAGE_VALUE_MASK;
}

// uint8_t* MessageReceiver::getMessageData(){
//   return messagedata;
// }
