#ifndef __ASIOSERIAL_H__
#define __ASIOSERIAL_H__

#include "Serial.h"

class AsioSerial : public Serial {
private:

public:
 AsioSerial(const std::string& port, int speed, bool verbose = true)
   : Serial(port, speed, verbose){
  }
 AsioSerial()
   : Serial(){
  }
  int connect();
  int connect(int fd);
  int disconnect();
  bool isConnected();
  size_t readSerial(unsigned char* data, size_t len);
  size_t writeSerial(unsigned char* data, size_t len);
private:
  void configure();
};

#endif  // __ASIOSERIAL_H__
