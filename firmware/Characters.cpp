#include "Characters.h"

#include "avr/pgmspace.h"

// #include "fonts/emptyfont.c"

// 5x8 fonts
#include "fonts/5x8.c" 
// #include "fonts/Misc-Fixed-Medium-R-Normal-8.c"
// #include "fonts/6x9.c" 

// #include "fonts/Misc-Fixed-Medium-R-Normal-7.c" // 506 bytes

// uint8_t* getCharacterData(char c){
//   uint8_t pos = 0;
//   while(typeface[pos].c != c && typeface[pos].c != '\0')
//     ++pos;
//   return typeface[pos].d;
// }

// void getCharacterData(char c, uint8_t* data){
//   uint16_t pos;
//   char m = pgm_read_byte_near(typeface);
//   while(m != c && m != '\0'){
//     pos += FONT_HEIGHT + 1;
//     m = pgm_read_byte_near(typeface + pos);
//   }
//   for(uint8_t i=0; i<FONT_HEIGHT; ++i)
//     data[i] = pgm_read_byte_near(typeface + pos + 1 + i);
// }


void getCharacterData(char c, uint8_t* data){
//   if(c > FONT_OFFSET && c < FONT_LENGTH+FONT_OFFSET){
  uint8_t index = c - FONT_OFFSET;
  if(index < FONT_LENGTH)
    for(uint8_t i=0; i<FONT_HEIGHT; ++i)
      data[i] = pgm_read_byte_near(typeface+index*FONT_HEIGHT+i);
  else
    for(uint8_t i=0; i<FONT_HEIGHT; ++i)
      data[i] = 0;
}

uint8_t getCharacterWidth(){
  return FONT_WIDTH;
}

uint8_t getCharacterHeight(){
  return FONT_HEIGHT;
}
