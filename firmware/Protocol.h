#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <inttypes.h>

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
  void sendParameterMessage(uint8_t pid, uint16_t value);
  void sendSensorMessage(uint8_t sid, uint16_t value);
  void sendPositionMessage(uint16_t x, uint16_t y);
  void sendReleaseMessage();
};

#endif /* _PROTOCOL_H_ */
