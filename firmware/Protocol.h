#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#ifndef MESSAGE_SENDER_TEST
#include <wiring.h>
#endif

#define POSITION_MSG       0x50 // 0x5 << 4
#define RELEASE_MSG        0x70 // 0x7 << 4
#define POT1_SENSOR_MSG    0x84 // 0x80 | (0x1 << 2)
#define X_SENSOR_MSG       0x88 // 0x80 | (0x2 << 2)
#define Y_SENSOR_MSG       0x8c // 0x80 | (0x3 << 2)
#define POT2_SENSOR_MSG    0x90 // 0x80 | (0x4 << 2)
#define BUTTON1_SENSOR_MSG 0x94 // 0x80 | (0x5 << 2)
#define BUTTON2_SENSOR_MSG 0x98 // 0x80 | (0x6 << 2)
#define BUTTON3_SENSOR_MSG 0x9c // 0x80 | (0x7 << 2)
#define PARAMETER_MSG      0xc0 // 0x3 << 6 B11000000

class Protocol {
public:
  virtual void sendParameterMessage(uint8_t pid, uint16_t value){}
  virtual void sendSensorMessage(uint8_t pid, uint16_t value){}
  virtual void sendPositionMessage(uint16_t x, uint16_t y){}
  virtual void sendReleaseMessage(){}
};

class SerialProtocol : public Protocol {
public:
  void sendParameterMessage(uint8_t pid, uint16_t value){
    // fixed length 2 bytes
    // parameter msg: 11ppppvv vvvvvvvv
    // 2 bits msg id, 4 bits parameter ID, 10 bits value
    serialWrite(PARAMETER_MSG | pid | ((value >> 8) & 0x03));
    serialWrite((uint8_t)value);
  }
  void sendSensorMessage(uint8_t sid, uint16_t value){
      // fixed length 2 bytes
      // sensor msg: 10ssssvv vvvvvvvv
      // 2 bits msg id, 4 bits sensor ID, 10 bits value
      serialWrite(sid | ((value >> 8) & 0x3));
      serialWrite(value & 0xff); //  & 0xff not needed?
  }
  void sendPositionMessage(uint16_t x, uint16_t y){
      // fixed length 3 bytes
      // xy msg: 0101xxxx xxxxxxyy yyyyyyyy
      serialWrite(POSITION_MSG | (x >> 6 & 0xf));
      serialWrite((x << 2 & 0xfc) | (y >> 8 & 0x3));
      serialWrite(y & 0xff);
  }
  void sendReleaseMessage(){
      // fixed length 1 byte
      // release msg: 01110000
      serialWrite(RELEASE_MSG);
  }
};

#endif /* _PROTOCOL_H_ */
