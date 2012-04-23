#include <string>
#include <iostream>
#include <unistd.h>
#include "Serial.h"
#include <boost/algorithm/string.hpp>

#include <stdio.h>
#include <util.h>

#include <stdlib.h>
#include <fcntl.h>

using namespace std;

class BlipCLI : public SerialCallback {
public:
  BlipCLI() : serial(NULL)
  {
    serial = Serial::createSerial();
    serial->setSpeed(57600);
  }
  bool isConnected(){
    return serial->isConnected();
  }
  const string getPort(){
    return serial->getPort();
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
    serial->setSerialCallback(this);
    serial->start();
//     serial->run(); // doesn't return, needs separate thread.
    while(running){
      unsigned char buf[128];
//       for(int i=0; i<sizeof(buf)){
      int i = 0;
      int val;
      do{
	std::cin >> val;
	buf[i] = val;
	std::cout << "collected 0x" << hex << val << std::endl;
      }while(val > 0 && val != '\n' && i++ < sizeof(buf));
      int sz = i-1;
      if(sz > 1)
	serial->writeSerial(buf, sz);
      std::cout << "sent " << sz << " bytes" << std::endl;
    }    
//       unsigned char buf[256];
//       size_t sz = serial->readSerial(buf, sizeof(buf));
//       for(int i=0; i<sz; ++i)
// 	serialInput(buf[i]);
//     }
  }
  void stop(){
    running = false;
  }
  Serial* serial;
private:
  volatile bool running;
};

void usage(){
  cout << "blipcli v0.1" << endl;
}

// int openPtty(){
//   int master;
//   int slave;
//   char buf[1024];
//   struct termios termp;
//   struct winsize winp;
//   int ret = openpty(&master, &slave, buf, &termp, &winp);
//   if(ret){
//     perror(buf);
//     std::cerr << "error opening pseudo tty: " << buf << std::endl;
//     return -1;
//   }
//       std::cout << "pseudo tty: " << buf << std::endl;
//   return master;
// }

int openPtty(){
  int ret = posix_openpt(O_RDWR);
  if(ret < 0){
    char buf[256];
    perror(buf);
    std::cerr << "error opening pseudo tty: " << buf << std::endl;
    return -1;
  }
  int master = ret;
  const char* name = ptsname(master);
  ret = grantpt(master);
  if(ret < 0){
    char buf[256];
    perror(buf);
    std::cerr << "error configuring pseudo tty: " << buf << std::endl;
    return -1;
  }
  std::cerr << "pseudo tty: " << name << std::endl;
  return master;
}

int main(int argc, char *argv[]) {
  BlipCLI service;
  for(int i=1; i<argc; ++i){
    if(boost::iequals(argv[i], "-p") && ++i < argc){
      service.setPort(argv[i]);
      service.serial->connect();
    }else if(boost::iequals(argv[i], "-c")){
      int master = openPtty();
      service.serial->connect(master);
 //      service.setFileDescriptor(master);
    }else if(boost::iequals(argv[i], "-h") || boost::iequals(argv[i], "--help")){
      usage();
      return 0;
    }else{
      usage();
      return -1;
    }
  }
  if(!service.isConnected()){
      usage();
      return -1;
  }
  service.run();
}
