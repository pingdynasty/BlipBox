#include <string>
#include <iostream>
#include <unistd.h>
#include "Serial.h"
#include <boost/algorithm/string.hpp>

using namespace std;

class BlipCLI : public SerialCallback {
public:
  BlipCLI() : serial(NULL)
  {
    serial = Serial::createSerial();
    serial->setSpeed(57600);
  }
  void setPort(const string& port){
    serial->setPort(port);
  }
  int handle(unsigned char* data, size_t len){
    for(int i=0; i<len; ++i)
      serialInput(data[i]);
  }
  void serialInput(unsigned char c){
    cout << "0x" << hex << (int)c << endl;
  }
  void run(){
    running = true;
    serial->connect();
    serial->setSerialCallback(this);
    serial->start();
    serial->run();
    while(running){
//       unsigned char buf[256];
//       size_t sz = serial->readSerial(buf, sizeof(buf));
//       for(int i=0; i<sz; ++i)
// 	serialInput(buf[i]);
    }
  }
  void stop(){
    running = false;
  }
private:
  volatile bool running;
  Serial* serial;
};

void usage(){
  cout << "blipcli v0.1" << endl;
}

int main(int argc, char *argv[]) {
  BlipCLI service;
  for(int i=1; i<argc; ++i){
    if(boost::iequals(argv[i], "-p") && ++i < argc){
      service.setPort(argv[i]);
    }else if(boost::iequals(argv[i], "-h") || boost::iequals(argv[i], "--help")){
      usage();
      return 0;
    }else{
      usage();
      return -1;
    }
  }
  service.run();
}
