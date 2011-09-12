#include "DisplayManager.h"

#include <stdlib.h>

#include "globals.h"
#include "Characters.h"

  // shifts the led data in the given direction
void DisplayManager::shift(uint8_t direction){
  // the leftmost 2 bits determine the direction: 0: left, 1: right, 2: up, 3: down
  // the rightmost 2 bits determines the number of steps: 1-4
  switch(direction & 0xc){
  case 0x0: // shift left
    for(uint8_t col=7; col>0; --col)
      for(uint8_t row=0; row<10; ++row)
        blipbox.leds.setLed(row, col, blipbox.leds.getLed(row, col-1));
    for(uint8_t row=0; row<10; ++row)
      blipbox.leds.setLed(row, 0, 0);
    break;
  case 0x4: // shift right
    for(uint8_t col=0; col<7; ++col)
      for(uint8_t row=0; row<10; ++row)
        blipbox.leds.setLed(row, col, blipbox.leds.getLed(row, col+1));
    for(uint8_t row=0; row<10; ++row)
      blipbox.leds.setLed(row, 7, 0);
    break;
  case 0x8:
    for(uint8_t col=0; col<8; ++col)
      for(uint8_t row=0; row<9; ++row)
        blipbox.leds.setLed(row, col, blipbox.leds.getLed(row+1, col));
    for(uint8_t col=0; col<8; ++col)
      blipbox.leds.setLed(9, col, 0);
    break;
  case 0xc:
    for(uint8_t col=0; col<8; ++col)
      for(uint8_t row=9; row>0; --row)
        blipbox.leds.setLed(row, col, blipbox.leds.getLed(row-1, col));
    for(uint8_t col=0; col<8; ++col)
      blipbox.leds.setLed(0, col, 0);
    break;
  }
}

void DisplayManager::printCharacter(char c, int8_t dx, int8_t dy, uint8_t brightness){
  uint8_t data[8];
  getCharacterData(c, data);
  for(uint8_t y=0; y<getCharacterHeight(); ++y){
    for(uint8_t x=0; x<getCharacterWidth(); ++x){
      // font data is 8 bits left adjusted
      if((data[y] & _BV(7-x)) && x+dx<10)
        blipbox.leds.setLed(x+dx, 7-y+dy, brightness);
      else if(x+dx<10)
        blipbox.leds.setLed(x+dx, 7-y+dy, 0x00);
    }
  }
}

// void DisplayManager::setDiagonalCross(uint8_t x, uint8_t y, uint8_t value){
//   //   x: 0-9, y: 0-7
// //   uint8_t d1 = row - col;
// //   uint8_t d2 = 7 - row - col;
// //   for(uint8_t i=0; i<8; ++i){
// //     if(i+d1 >= 0 && i+d1 < 10)
// // //       blipbox.leds.setLed(i+d1, i, value);
// //       blipbox.leds.setLed(i+d1, i, value * abs(i-col) / 7);
// //     if(i-d2 >= 0 && i-d2 < 10)
// // //       blipbox.leds.setLed(i-d2, 7-i, value);
// //       blipbox.leds.setLed(i-d2, 7-i, value * abs(7-i-col) / 7);
// //   }
//   for(int8_t i=0; i<8; ++i){
//     blipbox.leds.setLed(x+abs(y-i), i, value-(value/8)*abs(x-i));
//     blipbox.leds.setLed(x-abs(y-i), i, value-(value/8)*abs(x-i));
//   }    
// //   for(int8_t i=-8; i<8; ++i){
// //     blipbox.leds.setLed(x+i, y+i, value / (2*abs(i) + 1));
// //     blipbox.leds.setLed(x-i, y+i, value / (2*abs(i) + 1));
// //   }
// }

// // todo: set blob with 10 or 8 bit precision location
// void DisplayManager::setBlob(uint8_t x, uint8_t y, uint8_t value){
//   for(int8_t i=-3; i<4; i+=2){
//     blipbox.leds.setLed(x+i, y+i, value / (2*abs(i) + 1));
//     blipbox.leds.setLed(x+i, y-i, value / (2*abs(i) + 1));
//     blipbox.leds.setLed(x-i, y+i, value / (2*abs(i) + 1));
//     blipbox.leds.setLed(x-i, y-i, value / (2*abs(i) + 1));
//   }
// }

// void DisplayManager::setCross(uint8_t x, uint8_t y, uint8_t value){
//   for(uint8_t i=0; i<10; ++i)
//     blipbox.leds.setLed(i, y, value-(value/9)*abs(y-i));
//   for(uint8_t i=0; i<8; ++i)
//     blipbox.leds.setLed(x, i, value-(value/7)*abs(x-i));
// }

// void DisplayManager::setStar(uint8_t x, uint8_t y, uint8_t value){
//   blipbox.leds.setLed(x+1, y, value);
//   blipbox.leds.setLed(x, y+1, value);
//   blipbox.leds.setLed(x-1, y, value);
//   blipbox.leds.setLed(x, y-1, value);
// }

// void DisplayManager::setSquare(uint8_t x, uint8_t y, uint8_t value){
//   setStar(x, y, value);
//   value >>= 1;
//   blipbox.leds.setLed(x+1, y+1, value);
//   blipbox.leds.setLed(x-1, y+1, value);
//   blipbox.leds.setLed(x+1, y-1, value);
//   blipbox.leds.setLed(x-1, y-1, value);
// }

void DisplayManager::line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t brightness){
  // Bresenham's line algorithm
  // 248 bytes
  // todo: try Xiaolin Wu's anti-aliasing line algorithm
  int8_t dx, dy, inx, iny, e;	
  dx = x2 - x1;
  dy = y2 - y1;
  inx = dx > 0 ? 1 : -1;
  iny = dy > 0 ? 1 : -1;
  dx = abs(dx);
  dy = abs(dy);	
  if(dx >= dy) {
    dy <<= 1;
    e = dy - dx;
    dx <<= 1;
    while (x1 != x2) {
      blipbox.leds.setLed(x1, y1, brightness);
      if(e >= 0) {
	y1 += iny;
	e-= dx;
      }
      e += dy; x1 += inx;
    }
  }else{
    dx <<= 1;
    e = dx - dy;
    dy <<= 1;
    while (y1 != y2) {
      blipbox.leds.setLed(x1, y1, brightness);
      if(e >= 0) {
	x1 += inx;
	e -= dy;
      }
      e += dx; y1 += iny;
    }
  }
  blipbox.leds.setLed(x1, y1, brightness);
}

void DisplayManager::fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t brightness){
  for(; x1<x2; ++x1)
    for(uint8_t y=y1;y<y2; ++y)
      blipbox.leds.setLed(x1, y, brightness);
}
