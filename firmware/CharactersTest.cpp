#include "Characters.h"

#define _BV(bit) (1 << (bit))

#include <iostream>

using namespace std;

void printcgr(Character match){
  cout << match.c << ": " << endl;
  for(int i=0; i<5; ++i){
    for(int j=7; j>=0; --j){
      if(match.d[i] & _BV(j))
        cout << "*";
      else
        cout << "-";
    }
    cout << endl;
  }
}

// void printchar(char c){
//   int pos = 0;
//   cgr match = characters[pos++];
//   while(match.c != '\0'){
//     if(match.c == c){
//       printcgr(match);
//     }
//     match = characters[pos++];
//   }
// }

int main(int argc, const char* argv[]) {
  cout << "hello" << endl;
  for(int i=1; i<argc; ++i){
    int pos = 0;
    char c = argv[i][pos++];
    while(c != '\0'){
      Character cgr = getCharacter(c);
      printcgr(cgr);
      c = argv[i][pos++];
    }
    cout << endl;
  }
  cout << "done" << endl;
}
