#include "KeyController.h"
#include "TouchController.h"
#include "defs.h"
#include <stdint.h>

#define GRID_COLS 10
#define GRID_ROWS 8
// #define X_MIN 160
// #define X_MAX 860
// #define Y_MIN 130
// #define Y_MAX 850
// #define X_RANGE (X_MAX - X_MIN)
// #define Y_RANGE (Y_MAX - Y_MIN)

TouchController touch;

uint16_t touchscreen_x_min   = 160;
uint16_t touchscreen_x_range = 860 - touchscreen_x_min;
uint16_t touchscreen_y_min   = 160;
uint16_t touchscreen_y_range = 860 - touchscreen_y_min;

// void KeyController::init(){
//   touch.init();
// }

bool KeyController::keyscan(){
  uint8_t oldRow = row;
  uint8_t oldCol = col;
  this->update(); // sets row/col, calls check(), readX() and readY()
  if(this->getTouch() < SENSITIVITY){
    if(!pressed){
      // toggled from released to pressed
      pressed = true;
      return true;
    }else if(oldRow != row || oldCol != col){
      // different key pressed
      return true;
    }else{
      // same key, still pressed
      return false;
    }
  }else if(pressed){
    // toggled from pressed to released
    pressed = false;
    return true;
  }
  return false;
}

void KeyController::update(){
  this->check();
  row = this->readRow();
  if(row >= GRID_ROWS)
    row = GRID_ROWS - 1;
  col = this->readColumn();
  if(col >= GRID_COLS)
    col = GRID_COLS - 1;
}

uint8_t KeyController::readColumn(){
  return (this->readX() - touchscreen_x_min) * GRID_COLS / touchscreen_x_range;
}

uint8_t KeyController::readRow(){
  return (this->readY() - touchscreen_y_min) * GRID_ROWS / touchscreen_y_range;
}
