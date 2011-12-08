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

KeyController::KeyController() : 
  pos(), pressed(false), firstpos(0xf, 0xf), lastpos(0xf, 0xf), 
  lastpressed(LONG_MAX), lasttapped(LONG_MAX), lastreleased(LONG_MAX) {}

void KeyController::keyscan(){
  unsigned long now = millis();
  TouchEvent event;
  if(this->getZ() < blipbox.config.sensitivity){
    if(now - lastreleased < BOUNCE_THRESHOLD)
      return;
    this->update(); // sets x/y/row/col
    lastpos = pos;
    if(!pressed){
      // toggled from released to pressed
      event.type = PRESS_EVENT_TYPE;
      pressed = true;
      if(lastpos != firstpos)
        lasttapped = LONG_MAX;
      firstpos = lastpos;
      lastpressed = now;
    }else{
      // still pressed
      event.type = DRAG_EVENT_TYPE;
    }
    event.pos = &pos;
    blipbox.eventhandler->handle(event);
  }else if(pressed){
    if(now - lastpressed < BOUNCE_THRESHOLD)
      return;
    lastreleased = now;
    // toggled from pressed to released
    pressed = false;
    if(now - lasttapped < TAPTAP_THRESHOLD && lastpos == firstpos){
      event.type = TAPTAP_EVENT_TYPE;
      event.pos = &pos;
      blipbox.eventhandler->handle(event);
      lasttapped = LONG_MAX;
    }else if(now - lastpressed < TAP_THRESHOLD && lastpos == firstpos){
      event.type = TAP_EVENT_TYPE;
      event.pos = &pos;
      blipbox.eventhandler->handle(event);
      lasttapped = now;
    }
    event.pos = NULL;
    event.type = RELEASE_EVENT_TYPE;
    blipbox.eventhandler->handle(event);
  }
}

void KeyController::update(){
  pos.x = (uint16_t)(((getValue(1) - blipbox.config.touchscreen_x_min) / (float)blipbox.config.touchscreen_x_range)*SENSOR_MAX);
  // inverted range for y
  pos.y = (uint16_t)(((SENSOR_MAX - getValue(2) - blipbox.config.touchscreen_y_min) / (float)blipbox.config.touchscreen_y_range)*SENSOR_MAX);

  pos.setColumn(min(GRID_COLS - 1, pos.x * GRID_COLS / SENSOR_MAX));
  pos.setRow(min(GRID_ROWS - 1, pos.y * GRID_ROWS / SENSOR_MAX));
}
