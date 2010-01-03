// g++ DDSTest.cpp ../DDS.cpp -o test
// ./test 0101 ABCDEFGH ijklmnop
#include <iostream>
#include <math.h>

#define _BV(bit) (1 << (bit))
#include "../DDS.h"

using namespace std;

int main(int argc, const char* argv[]) {
  cout << "Testing DDS.cpp" << endl;
  DDS dds;
  dds.init();
  for(int i=1; i<argc; ++i){
    int freq = atoi(argv[i]);
    cout << "wavetable at frequency "<< freq << endl;
    dds.setFrequency(freq);
    for(int x=0; x<20; ++x)
      cout << (int)dds.getValue() << endl;
  }
  cout << "done" << endl;
}
