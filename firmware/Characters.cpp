#include "Characters.h"

#include "avr/pgmspace.h"

// #include "fonts/emptyfont.c"
// #include "fonts/5x8.c"

// sizes relevant only for [0-9a-zA-Z] font files
#include "fonts/6x9.c" // adds 578 bytes to program size
// #include "fonts/Misc-Fixed-Medium-R-Normal-7.c" // 506 bytes
// #include "fonts/Misc-Fixed-Medium-R-Normal-8.c" // 578 bytes

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


// If FONT_HEIGHT * 127 bytes (1kb for 6x9 font)
// then 7-bit ascii can be directly indexed.
// The first 32 chars could be discarded/replaced.
// It saves 1 byte per char - 6x9.c defines 90 chars, in 90*8+90 = 810 bytes.
// Hex size reduced by 154 bytes when using fixed array for ascii chars 40-122 (83 chars)

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
