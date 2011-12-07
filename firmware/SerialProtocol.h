#ifndef _SERIAL_PROTOCOL_H_
#define _SERIAL_PROTOCOL_H_

#include <inttypes.h>

void sendParameterMessage(uint8_t pid, uint16_t value);
void sendSensorMessage(uint8_t pid, uint16_t value);
void sendPositionMessage(uint16_t x, uint16_t y);
void sendReleaseMessage();

#endif /* _SERIAL_PROTOCOL_H_ */
