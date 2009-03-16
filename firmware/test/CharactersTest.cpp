// g++ CharactersTest.cpp ../Characters.cpp -o test
// ./test 0101 ABCDEFGH ijklmnop
#include <iostream>
using namespace std;

#define _BV(bit) (1 << (bit))
#include "../Characters.h"

void printGlyph(char c, uint8_t* pixels){
  cout << c << ": " << endl;
  for(int i=0; i<getCharacterHeight(); ++i){
    for(int j=7; j>=(8-getCharacterWidth()); --j)
      cout << (pixels[i] & _BV(j) ? "*" : "-");
    cout << endl;
  }
}

int main(int argc, const char* argv[]) {
  cout << "Testing Characters.cpp" << endl;
  for(int i=1; i<argc; ++i){
    int pos = 0;
    char c = argv[i][pos++];
    while(c != '\0'){
      uint8_t* chr = getCharacterData(c);
      printGlyph(c, chr);
      c = argv[i][pos++];
    }
    cout << endl;
  }
  cout << "done" << endl;
}
