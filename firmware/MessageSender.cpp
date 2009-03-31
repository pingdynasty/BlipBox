#include "MessageSender.h"

#ifndef MESSAGE_SENDER_TEST
#include <wiring.h>
#endif

#include "defs.h"

#define XY_MSG             0x50 // 0x5 << 4
#define RELEASE_MSG        0x70 // 0x7 << 4
#define TOUCH_SENSOR_MSG   0x84 // 0x80 | (0x1 << 2)
#define X_SENSOR_MSG       0x88 // 0x80 | (0x2 << 2)
#define Y_SENSOR_MSG       0x8c // 0x80 | (0x3 << 2)
#define POT_SENSOR_MSG     0x90 // 0x80 | (0x4 << 2)
#define BUTTON1_SENSOR_MSG 0x94 // 0x80 | (0x5 << 2)
#define BUTTON2_SENSOR_MSG 0x98 // 0x80 | (0x6 << 2)
#define BUTTON3_SENSOR_MSG 0x9c // 0x80 | (0x7 << 2)
#define PING_SENSOR_MSG    0xa0 // 0x80 | (0x8 << 2)

#define X_SENSOR           0 // shares the sensordata slot with XY_SENSOR
#define Y_SENSOR           1 // shares the sensordata slot with RELEASE_SENSOR

uint8_t sensorids[SENSOR_COUNT] = {
  XY_MSG,
  RELEASE_MSG,
  TOUCH_SENSOR_MSG,
  POT_SENSOR_MSG,
  BUTTON1_SENSOR_MSG,
  BUTTON2_SENSOR_MSG,
  BUTTON3_SENSOR_MSG
};

void MessageSender::init(){
}

void MessageSender::updateXY(uint16_t x, uint16_t y, uint16_t z){
//     // prepare to send xy message
//     sensordata[XY_SENSOR] = SENSOR_MAX - keys.getX();
//     // we borrow XY and RELEASE message data slots to store X and Y positions
//     sensordata[RELEASE_SENSOR] = keys.getY();
//     sensordata[TOUCH_SENSOR] = keys.getTouch();
//     sensorstatus |= _BV(XY_SENSOR);  // set to send xy message
//     sensorstatus &= ~_BV(RELEASE_SENSOR); // suppress pending release message
  sensordata[X_SENSOR] = x;
  sensordata[Y_SENSOR] = y;
  sensordata[TOUCH_SENSOR] = z;
  sensorstatus |= _BV(XY_SENSOR);  // set to send xy message
  sensorstatus &= ~_BV(RELEASE_SENSOR); // suppress pending release message
}


void MessageSender::updateRelease(){
  if(sensordata[TOUCH_SENSOR] != SENSOR_MAX){
    sensordata[TOUCH_SENSOR] = SENSOR_MAX;
    // we've gone from pressed to unpressed
    // prepare to send release message
    sensorstatus &= ~_BV(XY_SENSOR); // suppress pending xy message
    sensorstatus |= _BV(RELEASE_SENSOR);  // set to send release message
  }
}

bool MessageSender::updateSensor(uint8_t index, uint16_t value){
  if(sensordata[index] != value){
    sensordata[index] = value;
    sensorstatus |= _BV(index);
    return true;
  }
  return false;
}

void MessageSender::sendReleaseMessage(){
  // fixed length 1 byte
  // release msg: 01110000
  serialWrite(RELEASE_MSG);
}

void MessageSender::sendXYMessage(){
  // fixed length 3 bytes
  // xy msg: 0101xxxx xxxxxxyy yyyyyyyy
  serialWrite(XY_MSG | (sensordata[X_SENSOR] >> 6 & 0xf));
  serialWrite((sensordata[X_SENSOR] << 2 & 0xfc) | (sensordata[Y_SENSOR] >> 8 & 0x3));
  serialWrite(sensordata[Y_SENSOR] & 0xff);
}

void MessageSender::sendSensorMessage(){
  // fixed length 2 bytes
  // sensor msg: 10ssssvv vvvvvvvv
  // writes the sensor message at the current writepos position
  // 2 bits msg id, 4 bits sensor ID, 10 bits value
  serialWrite(sensorids[writepos] | ((sensordata[writepos] >> 8) & 0x3));
  serialWrite(sensordata[writepos] & 0xff); //  & 0xff not needed?
}

void MessageSender::sendNextMessage(){
  // writes at most one message, the next one in line, then returns
  for(; sensorstatus; ){
    // increment early to avoid sending same type msg over and over
    if(++writepos == SENSOR_COUNT)
      writepos = 0;
    if(sensorstatus & _BV(writepos)){
      switch(writepos){
      case XY_SENSOR:
        sendXYMessage();
        break;
      case RELEASE_SENSOR:
        sendReleaseMessage();
        break;
      default:
        sendSensorMessage();
      }
      sensorstatus &= ~_BV(writepos);
      break;
    }
  }
}
