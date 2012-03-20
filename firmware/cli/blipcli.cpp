#include <string>
#include <iostream>
#include <unistd.h>
#include "AsioSerial.h"
#include <boost/algorithm/string.hpp>

using namespace std;

class BlipCLI {
public:
  BlipCLI() : serial(new AsioSerial())
  {
    serial->setSpeed(57600);
  }
  void setPort(const string& port){
    serial->setPort(port);
  }

private:
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
}
