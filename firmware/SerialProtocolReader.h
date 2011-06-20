#ifndef _SERIALPROTOCOLREADER_H_
#define _SERIALPROTOCOLREADER_H_

#include "SerialReader.h"

class SerialProtocolReader : public SerialReader {
  void serialInput(unsigned char c);
};

#endif /* _SERIALPROTOCOLREADER_H_ */
