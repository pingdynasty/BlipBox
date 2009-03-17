#include "Characters.h"

// #include "fonts/emptyfont.c"
#include "fonts/6x9.c" // adds 578 bytes to program size
// #include "fonts/Misc-Fixed-Medium-R-Normal-7.c" // 506 bytes
// #include "fonts/Misc-Fixed-Medium-R-Normal-8.c" // 578 bytes

uint8_t* getCharacterData(char c){
  uint8_t pos = 0;
  while(typeface[pos].c != c && typeface[pos].c != '\0')
    ++pos;
  return typeface[pos].d;
}

uint8_t getCharacterWidth(){
  return FONT_WIDTH;
}

uint8_t getCharacterHeight(){
  return FONT_HEIGHT;
}
