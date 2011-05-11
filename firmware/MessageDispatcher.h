#ifndef _MESSAGEDISPATCHER_H_
#define _MESSAGEDISPATCHER_H_

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

#define NO_MESSAGE         -1

class AbstractMessage {
public:
//   virtual bool send() = 0;
  virtual bool send() {}; // the empty function def adds 8 bytes to data!
};

class ParameterMessage : public AbstractMessage {
private:
  uint8_t m_pid;
  int16_t m_value;
public:
//   ParameterMessage(uint8_t pid) : m_pid(pid), m_value(NO_MESSAGE) {}
  ParameterMessage() : m_value(NO_MESSAGE) {}
//   void update(uint16_t value){
//     m_value = value;
//   }
  bool update(uint8_t pid, uint16_t value){
    m_pid = pid;
    m_value = value;
  }
  bool send(){
    if(m_value != NO_MESSAGE){
      // fixed length 2 bytes
      // parameter msg: 11ppppvv vvvvvvvv
      // 2 bits msg id, 4 bits parameter ID, 10 bits value
      serialWrite(PARAMETER_MSG | m_pid | ((m_value >> 8) & 0x03));
      serialWrite((uint8_t)m_value);
      m_value = NO_MESSAGE;
      return true;
    }
    return false;
  }
};

class SensorMessage : public AbstractMessage {
private:
  bool m_status;
  uint8_t m_sid;
  int16_t m_value;
public:
  SensorMessage(uint8_t sid) : m_sid(sid), m_value(NO_MESSAGE) {}
  void update(uint16_t value){
    if(m_value  != value){
      m_value = value;
      m_status = true;
    }
  }
  bool send(){
    if(m_status){
      // fixed length 2 bytes
      // sensor msg: 10ssssvv vvvvvvvv
      // 2 bits msg id, 4 bits sensor ID, 10 bits value
      serialWrite(m_sid | ((m_value >> 8) & 0x3));
      serialWrite(m_value & 0xff); //  & 0xff not needed?
      m_status = false;
      return true;
    }
    return false;
  }
};

class ReleaseMessage : public AbstractMessage {
private:
  int8_t m_value;
public:
  ReleaseMessage() : m_value(NO_MESSAGE) {}
  void update(){
    m_value = 1;
  }
  bool send(){
    if(m_value != NO_MESSAGE){
      // fixed length 1 byte
      // release msg: 01110000
      serialWrite(RELEASE_MSG);
      m_value = NO_MESSAGE;
      return true;
    }
    return false;
  }
};

class PositionMessage : public AbstractMessage {
private:
  bool m_status;
  int16_t m_x;
  int16_t m_y;
public:
//   PositionMessage() : m_x(NO_MESSAGE) {}
  void update(uint16_t x, uint16_t y){
    if(m_x != x || m_y != y){
      m_x = x;
      m_y = y;
      m_status = true;
    }
  }
  void reset(){
    m_status = false;
    m_x = m_y = 0;
  }
  bool send(){
    if(m_status){
      // fixed length 3 bytes
      // xy msg: 0101xxxx xxxxxxyy yyyyyyyy
      serialWrite(POSITION_MSG | (m_x >> 6 & 0xf));
      serialWrite((m_x << 2 & 0xfc) | (m_y >> 8 & 0x3));
      serialWrite(m_y & 0xff);
      m_status = false;
      return true;
    }
    return false;
  }
};

#define MESSAGE_TYPE_COUNT 4

class MessageDispatcher {
private:
  AbstractMessage* msgs[MESSAGE_TYPE_COUNT];
  uint8_t index;
public:
  PositionMessage position;
  ReleaseMessage release;
  SensorMessage button1;
  SensorMessage button2;
  ParameterMessage parameter;
  MessageDispatcher() :
    button1(BUTTON1_SENSOR_MSG),
    button2(BUTTON2_SENSOR_MSG)
  {
    msgs[0] = &position;
    msgs[1] = &release;
    msgs[2] = &button1;
    msgs[3] = &button2;
  }
  void init(){}
  bool send(){
    uint8_t cnt = MESSAGE_TYPE_COUNT;
    while(cnt--){
      if(++index == MESSAGE_TYPE_COUNT)
        index = 0;
      if(msgs[index]->send())
        return true;
    }
    return false;
  }
};

#endif /* _MESSAGEDISPATCHER_H_ */
