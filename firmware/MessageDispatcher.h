#ifndef _MESSAGEDISPATCHER_H_
#define _MESSAGEDISPATCHER_H_

#include "Protocol.h"
/* #include "MidiZoneProtocol.h" */

#define NO_MESSAGE         -1

extern Protocol* protocol;

class AbstractMessage {
public:
//   virtual bool send() = 0;
  virtual bool send() { return false; }; // the default function def adds a few bytes of data
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
  void update(uint8_t pid, uint16_t value){
    m_pid = pid;
    m_value = value;
  }
  bool send(){
    if(m_value != NO_MESSAGE){
      protocol->sendParameterMessage(m_pid, m_value);
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
      protocol->sendSensorMessage(m_sid, m_value);
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
      protocol->sendReleaseMessage();
      m_value = NO_MESSAGE;
      return true;
    }
    return false;
  }
  void reset(){
    m_value = NO_MESSAGE;
  }
};

class PositionMessage : public AbstractMessage {
private:
  bool m_status;
  uint16_t m_x;
  uint16_t m_y;
public:
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
      protocol->sendPositionMessage(m_x, m_y);
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
/*   MidiProtocol midiprotocol; */
public:
/*   MidiZoneProtocol midiprotocol; */
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
/*     midiprotocol.loadPreset(0); */
  }
  void setSerialProtocol(){
    protocol = &serialprotocol;
  }
/*   void setMidiProtocol(){ */
/*     protocol = &midiprotocol; */
/*   } */
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
