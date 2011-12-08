#ifndef _SERIALPROTOCOLREADER_H_
#define _SERIALPROTOCOLREADER_H_

#include "SerialReader.h"

#define SERIAL_RX_BUFFER_SIZE 3

class SerialProtocolReader : public SerialReader {
public:
  void serialInput(unsigned char c);
private:
  uint8_t getMessageType();
  uint16_t getTwelveBitValue();
  uint16_t getTenBitValue();
  uint8_t getFourBitValue();
  void handleMidiPresetCommand();
  void handleSetLedMessage();
  void handleSetLedRowMessage();  
  void handleSetLedColumnMessage();
  void handleWriteCharacterMessage();
  void handleSetParameterMessage();
  void handleFillMessage();
  void handleShiftLedsMessage();
  void handleCommandMessage();

  unsigned char rx_buffer[SERIAL_RX_BUFFER_SIZE];
  uint8_t rx_buffer_head;
  bool autoflip;
};

#endif /* _SERIALPROTOCOLREADER_H_ */
