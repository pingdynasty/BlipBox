#ifndef _BUFFERED_SERIAL_READER_H_
#define _BUFFERED_SERIAL_READER_H_

#include "SerialReader.h"

template<int RX_BUFFER_SIZE>
class BufferedSerialReader : public SerialReader {
public:
 BufferedSerialReader() : rx_buffer_head(0), rx_buffer_tail(0) {}
  int serialAvailable() {
    return (RX_BUFFER_SIZE + rx_buffer_head - rx_buffer_tail) % RX_BUFFER_SIZE;
  }
  int serialRead() {
    // if the head isn't ahead of the tail, we don't have any characters
    if (rx_buffer_head == rx_buffer_tail) {
      return -1;
    } else {
      unsigned char c = rx_buffer[rx_buffer_tail];
      rx_buffer_tail = (rx_buffer_tail + 1) % RX_BUFFER_SIZE;
      return c;
    }
  }
  virtual void serialInput(unsigned char c){
	int i = (rx_buffer_head + 1) % RX_BUFFER_SIZE;
	if (i != rx_buffer_tail) {
		rx_buffer[rx_buffer_head] = c;
		rx_buffer_head = i;
	}
  }
  void flush(){
    rx_buffer_head = rx_buffer_tail;
  }
  unsigned char volatile rx_buffer[RX_BUFFER_SIZE];
  volatile int rx_buffer_head;
  volatile int rx_buffer_tail;
};

#endif /* _BUFFERED_SERIAL_READER_H_ */
