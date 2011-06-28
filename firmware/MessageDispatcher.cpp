#include "MessageDispatcher.h"

#include "globals.h"

bool ParameterMessage::send(){
  if(m_value != NO_MESSAGE){
    blipbox.sender.protocol->sendParameterMessage(m_pid, m_value);
    m_value = NO_MESSAGE;
    return true;
  }
  return false;
}

bool SensorMessage::send(){
  if(m_value != NO_MESSAGE){
    blipbox.sender.protocol->sendSensorMessage(m_sid, m_value);
    m_value = NO_MESSAGE;
    return true;
  }
  return false;
}

bool ReleaseMessage::send(){
  if(m_value != NO_MESSAGE){
    blipbox.sender.protocol->sendReleaseMessage();
    m_value = NO_MESSAGE;
    return true;
  }
  return false;
}

bool PositionMessage::send(){
  if(m_x != NO_MESSAGE){
    blipbox.sender.protocol->sendPositionMessage(m_x, m_y);
    m_x = NO_MESSAGE;
    return true;
  }
  return false;
}

