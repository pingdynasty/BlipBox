#include "KeyController.h"
#include "TouchController.h"
#include "globals.h"
#include <stdint.h>

#define GRID_COLS 10
#define GRID_ROWS 8

// x/y readings from four corners of P4 screen
// 808/218 br
// 806/784 tr
// 257/783 tl
// 256/212 bl

// x/y readings from four corners of V6 screen
// 808/210
// 802/774
// 258/783
// 251/210

// void KeyController::init(){
//   TouchController::init();
// }

// bool KeyController::isPressed(){
//   return this->getZ() < blipbox.config.sensitivity;
// }

KeyState KeyController::keyscan(){
  uint8_t oldRow = getPosition().row;
  uint8_t oldCol = getPosition().column;
  if(this->getZ() < blipbox.config.sensitivity){
    this->update(); // sets row/col
    if(!pressed){
      // toggled from released to pressed
      pressed = true;
      return PRESSED;
    }else if(oldRow != getPosition().row || 
	     oldCol != getPosition().column){
      // different key pressed
      return DRAGGED;
    }else{
      // same key, still pressed
      return UNCHANGED;
    }
  }else if(pressed){
    // toggled from pressed to released
    pressed = false;
    return RELEASED;
  }else{
    return DISENGAGED;
  }
}

void KeyController::update(){
  getPosition().column = getPosition().x * GRID_COLS / SENSOR_MAX;
  if(getPosition().column >= GRID_COLS)
    getPosition().column = GRID_COLS - 1;
  getPosition().row = getPosition().y * GRID_ROWS / SENSOR_MAX;
  if(getPosition().row >= GRID_ROWS)
    getPosition().row = GRID_ROWS - 1;
}
