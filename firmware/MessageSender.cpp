#include "MessageSender.h"

#ifndef MESSAGE_SENDER_TEST
#include <wiring.h>
#endif

#include "globals.h"
#include "defs.h"
#include "Parameters.h"

#define XY_MSG             0x50 // 0x5 << 4
#define RELEASE_MSG        0x70 // 0x7 << 4
#define POT1_SENSOR_MSG    0x84 // 0x80 | (0x1 << 2)
#define X_SENSOR_MSG       0x88 // 0x80 | (0x2 << 2)
#define Y_SENSOR_MSG       0x8c // 0x80 | (0x3 << 2)
#define POT2_SENSOR_MSG    0x90 // 0x80 | (0x4 << 2)
#define BUTTON1_SENSOR_MSG 0x94 // 0x80 | (0x5 << 2)
#define BUTTON2_SENSOR_MSG 0x98 // 0x80 | (0x6 << 2)
#define BUTTON3_SENSOR_MSG 0x9c // 0x80 | (0x7 << 2)
#define PARAMETER_MSG      0xc0 // 0x3 << 6 B11000000

#define X_SENSOR           0 // shares the sensordata slot with XY_SENSOR
#define Y_SENSOR           1 // shares the sensordata slot with XY_SENSOR
#define Z_SENSOR           2 // shares the sensordata slot with RELEASE_SENSOR

// sensorid indices must mach sensorvalue indices
uint8_t sensorids[SENSOR_COUNT] = {
  XY_MSG, // XY_SENSOR
  XY_MSG,
  RELEASE_MSG,  // RELEASE_SENSOR
  POT1_SENSOR_MSG,  // POT1_SENSOR
  POT2_SENSOR_MSG,  // POT2_SENSOR
  BUTTON1_SENSOR_MSG,
  BUTTON2_SENSOR_MSG,
  BUTTON3_SENSOR_MSG
};

void MessageSender::init(){
}

void MessageSender::sendConfigurationParameters(){
  error(1);
  serialWrite(PARAMETER_MSG | getParameter(SENSITIVITY_PARAMETER_ID));
  serialWrite(PARAMETER_MSG | getParameter(BRIGHTNESS_PARAMETER_ID));
  serialWrite(PARAMETER_MSG | getParameter(TLC_GSCLK_PERIOD_PARAMETER_ID));
  serialWrite(PARAMETER_MSG | getParameter(SERIAL_SPEED_PARAMETER_ID));
}

void MessageSender::updateXY(uint16_t x, uint16_t y, uint16_t z){
  updateSensor(X_SENSOR, x);
  updateSensor(Y_SENSOR, y);
  sensordata[Z_SENSOR] = z;
  sensorstatus &= ~_BV(RELEASE_SENSOR); // suppress pending release message
}

void MessageSender::updateRelease(){
  updateSensor(Z_SENSOR, SENSOR_MAX);
  sensorstatus &= ~_BV(X_SENSOR); // suppress pending xy message
  sensorstatus &= ~_BV(Y_SENSOR); // suppress pending xy message
}

bool MessageSender::updateSensor(uint8_t index, uint16_t value){
  if(sensordata[index] != value){
    if(sensorstatus & _BV(index))
      // if last value has not been sent yet,
      // average current with previous value: x = x0/2+x1/2
      sensordata[index] = value/2+sensordata[index]/2;
    else
      // otherwise reset to current value
      sensordata[index] = value;
    // indicate that new data is available to be sent
    sensorstatus |= _BV(index);
    return true;
  }
  return false;
}

void MessageSender::sendReleaseMessage(){
  // fixed length 1 byte
  // release msg: 01110000
  serialWrite(RELEASE_MSG);
  sensorstatus &= ~_BV(Z_SENSOR);
}

void MessageSender::sendXYMessage(){
  // fixed length 3 bytes
  // xy msg: 0101xxxx xxxxxxyy yyyyyyyy
  serialWrite(XY_MSG | (sensordata[X_SENSOR] >> 6 & 0xf));
  serialWrite((sensordata[X_SENSOR] << 2 & 0xfc) | (sensordata[Y_SENSOR] >> 8 & 0x3));
  serialWrite(sensordata[Y_SENSOR] & 0xff);
  sensorstatus &= ~_BV(X_SENSOR); // cancel pending y message
  sensorstatus &= ~_BV(Y_SENSOR); // cancel pending y message
}

void MessageSender::sendSensorMessage(){
  // fixed length 2 bytes
  // sensor msg: 10ssssvv vvvvvvvv
  // writes the sensor message at the current writepos position
  // 2 bits msg id, 4 bits sensor ID, 10 bits value
  serialWrite(sensorids[writepos] | ((sensordata[writepos] >> 8) & 0x3));
  serialWrite(sensordata[writepos] & 0xff); //  & 0xff not needed?
  // indicate that data has been sent
  sensorstatus &= ~_BV(writepos);
}

void MessageSender::sendNextMessage(){
  // writes at most one message, the next one in line, then returns
  for(; sensorstatus; ){
    // increment early to avoid sending same type msg over and over
    if(++writepos == SENSOR_COUNT)
      writepos = 0;
    if(sensorstatus & _BV(writepos)){
      switch(writepos){
      case X_SENSOR:
      case Y_SENSOR:
        sendXYMessage();
        break;
      case Z_SENSOR:
        sendReleaseMessage();
        break;
      default:
        sendSensorMessage();
      }
      // message sent; exit loop
      break;
    }
  }
}
