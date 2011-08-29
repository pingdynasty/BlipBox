#ifndef _MESSAGEDISPATCHER_H_
#define _MESSAGEDISPATCHER_H_

#include "Protocol.h"
#include "Position.h"

#define NO_MESSAGE        -1

class AbstractMessage {
public:
  virtual bool send() { 
    return false; 
  };
};

class ParameterMessage : public AbstractMessage {
private:
  uint8_t m_pid;
  int16_t m_value;
public:
  ParameterMessage() : m_value(NO_MESSAGE) {}
  void update(uint8_t pid, uint16_t value){
    m_pid = pid;
    m_value = value;
  }
  bool send();
};

class SensorMessage : public AbstractMessage {
private:
  uint8_t m_sid;
  int16_t m_value;
public:
  SensorMessage(uint8_t sid) : m_sid(sid), m_value(NO_MESSAGE) {}
  void update(int16_t value){
    if(m_value != value)
      m_value = value;
  }
  bool send();
};

class ReleaseMessage : public AbstractMessage {
private:
  int8_t m_value;
public:
  ReleaseMessage() : m_value(NO_MESSAGE) {}
  void update(){
    m_value = 1;
  }
  bool send();
  void reset(){
    m_value = NO_MESSAGE;
  }
};

class PositionMessage : public AbstractMessage {
private:
  int16_t m_x;
  uint16_t m_y;
public:
  PositionMessage() : m_x(NO_MESSAGE) {}
  void update(const Position& pos){
    if(m_x != (int16_t)pos.x || m_y != pos.y){
      m_x = pos.x;
      m_y = pos.y;
    }
  }
  void reset(){
    m_x = NO_MESSAGE;
  }
  bool send();
};

#define MESSAGE_TYPE_COUNT 4

class MessageDispatcher {
private:
  AbstractMessage* msgs[MESSAGE_TYPE_COUNT];
  uint8_t index;
public:
  Protocol* protocol;
  SerialProtocol serialprotocol;
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
  void init(){
    protocol = &serialprotocol;
  }
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
