#ifndef _SERIALPROTOCOLREADER_H_
#define _SERIALPROTOCOLREADER_H_

#include "SerialReader.h"

class SerialProtocolReader : public SerialReader {
public:
  void serialInput(unsigned char c);
private:
};

#endif /* _SERIALPROTOCOLREADER_H_ */
