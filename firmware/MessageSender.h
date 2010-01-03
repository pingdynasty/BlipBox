#ifndef _MESSAGESENDER_H_
#define _MESSAGESENDER_H_

// new protocol: 1, 2 or 3 bytes
// touch position x/y
// 4 bits msg id
// 10 bits x
// 10 bits y
// xy msg: 0101xxxx xxxxxxyy yyyyyyyy
// release msg: 01110000
// sensor msg: 10ssssvv vvvvvvvv where ssss is sensor id bits
// key change ? nope
  // key change: 4 bits id, 4 bits empty, 8 bits new location
  // mode change message
  // todo: key press/release/change messages

#include <inttypes.h>

#define XY_SENSOR          0
#define RELEASE_SENSOR     2
#define POT1_SENSOR        3
#define POT2_SENSOR        4
#define BUTTON1_SENSOR     5
#define BUTTON2_SENSOR     6
#define BUTTON3_SENSOR     7

#define SENSOR_COUNT 8 // todo: make sensordata and sensorids configurable for different devices (Taquito, BlipBox)

class MessageSender {

private:

  uint8_t sensorstatus; // bits set to 1 if the value has changed
  uint16_t sensordata[SENSOR_COUNT];
  uint8_t writepos;

public:

  void init();

  void updateXY(uint16_t x, uint16_t y, uint16_t z);

  void updateRelease();

  bool updateSensor(uint8_t index, uint16_t value);

  void sendNextMessage();

private:

  void sendReleaseMessage();

  void sendXYMessage();

  void sendSensorMessage();

};


#endif /* _MESSAGESENDER_H_ */
