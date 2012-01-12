#ifndef _MIDIMESSAGEREADER_H_
#define _MIDIMESSAGEREADER_H_

#include "MidiInterface.h"
#include "SerialReader.h"

class MidiMessageReader : public SerialReader, public MidiInterface {
public:
  MidiMessageReader();
  void serialInput(unsigned char c);
private:
  uint8_t buf[3];
  uint8_t pos;
  uint8_t len;
};

#endif /* _MIDIMESSAGEREADER_H_ */
