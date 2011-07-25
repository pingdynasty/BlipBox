#include "KeyController.h"
#include "TouchController.h"
#include "globals.h"
#include "serial.h"
#include <limits.h>
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

#define BOUNCE_THRESHOLD 20
#define TAP_THRESHOLD    300 // ms between press and release to be considered tap
#define TAPTAP_THRESHOLD 600 // ms between taps to be considered double-tap
unsigned long lastpressed   = LONG_MAX;
unsigned long lasttapped    = LONG_MAX;
unsigned long lastreleased  = LONG_MAX;
uint8_t       firstpos      = 0xff;
uint8_t       lastpos       = 0xff;

void KeyController::keyscan(){
  unsigned long now = millis();
  if(this->getZ() < blipbox.config.sensitivity){
    if(now - lastreleased < BOUNCE_THRESHOLD)
      return;
    this->update(); // sets x/y/row/col
    lastpos = (pos.column << 4) | pos.row;
    if(!pressed){
      // toggled from released to pressed
      pressed = true;
      if(lastpos != firstpos)
        lasttapped = LONG_MAX;
      firstpos = lastpos;
      blipbox.eventhandler->press(pos);
      lastpressed = now;
    }else{
      // still pressed
      blipbox.eventhandler->drag(pos);
    }
  }else if(pressed){
    if(now - lastpressed < BOUNCE_THRESHOLD)
      return;
    lastreleased = now;
    // toggled from pressed to released
    pressed = false;
    if(now - lasttapped < TAPTAP_THRESHOLD && lastpos == firstpos){
      blipbox.eventhandler->taptap(pos);
      lasttapped = LONG_MAX;
    }else if(now - lastpressed < TAP_THRESHOLD && lastpos == firstpos){
      blipbox.eventhandler->tap(pos);
      lasttapped = now;
    }
    blipbox.eventhandler->release(pos);
    return;
  }
}

void KeyController::update(){
  pos.x = (uint16_t)(((getValue(1) - blipbox.config.touchscreen_x_min) / (float)blipbox.config.touchscreen_x_range)*SENSOR_MAX);
  // inverted range for y
  pos.y = (uint16_t)(((SENSOR_MAX - getValue(2) - blipbox.config.touchscreen_y_min) / (float)blipbox.config.touchscreen_y_range)*SENSOR_MAX);

  pos.column = pos.x * GRID_COLS / SENSOR_MAX;
  if(pos.column >= GRID_COLS)
    pos.column = GRID_COLS - 1;
  pos.row = pos.y * GRID_ROWS / SENSOR_MAX;
  if(pos.row >= GRID_ROWS)
    pos.row = GRID_ROWS - 1;
}
