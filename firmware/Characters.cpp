#include "Characters.h"

#include "fonts/6x9.c"
// #include "fonts/Misc-Fixed-Medium-R-Normal-8.c"

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
