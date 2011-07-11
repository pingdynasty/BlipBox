#include "Protocol.h"
#include "serial.h"

void SerialProtocol::sendParameterMessage(uint8_t pid, uint16_t value){
  // fixed length 2 bytes
  // parameter msg: 11ppppvv vvvvvvvv
  // 2 bits msg id, 4 bits parameter ID, 10 bits value
  serialWrite(PARAMETER_MSG | pid | ((value >> 8) & 0x03));
  serialWrite((uint8_t)value);
}
void SerialProtocol::sendSensorMessage(uint8_t sid, uint16_t value){
  // fixed length 2 bytes
  // sensor msg: 10ssssvv vvvvvvvv
  // 2 bits msg id, 4 bits sensor ID, 10 bits value
  serialWrite(sid | ((value >> 8) & 0x3));
  serialWrite(value & 0xff); //  & 0xff not needed?
}
void SerialProtocol::sendPositionMessage(uint16_t x, uint16_t y){
  // fixed length 3 bytes
  // xy msg: 0101xxxx xxxxxxyy yyyyyyyy
  serialWrite(POSITION_MSG | (x >> 6 & 0xf));
  serialWrite((x << 2 & 0xfc) | (y >> 8 & 0x3));
  serialWrite(y & 0xff);
}
void SerialProtocol::sendReleaseMessage(){
  // fixed length 1 byte
  // release msg: 01110000
  serialWrite(RELEASE_MSG);
}
