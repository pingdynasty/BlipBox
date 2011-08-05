#include "LedController.h"

void LedController::init(){
  back_buffer = buf1;
  counter.init();
}

void LedController::flip(){
//   uint8_t* front_buffer = flipped ? buf2 : buf1;
//   back_buffer = flipped ? buf1 : buf2;  
//   // memcpy(to, from, bytes)
//   memcpy(back_buffer, front_buffer, LED_BUFFER_LENGTH);
//   flipped = !flipped;
}

void LedController::displayCurrentRow(void){
  counter.tick();
}

void LedController::sendBufferData(uint8_t row){
}
