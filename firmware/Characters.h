#ifndef _CHARACTERS_H_
#define _CHARACTERS_H_

#include <stdint.h>

// class Character { 
// public:
//   char c;
//   uint8_t d[5];
// };

// class Characters {
// public:
/* uint8_t* getCharacterData(char c); */
void getCharacterData(char c, uint8_t* data);
uint8_t getCharacterWidth();
uint8_t getCharacterHeight();
// };

#endif /* _CHARACTERS_H_ */
