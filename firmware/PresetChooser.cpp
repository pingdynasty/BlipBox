#include <stdint.h>
#include "globals.h"
#include "PresetChooser.h"

#define MAX_PRESET 5

void PresetChooser::init(){
  if(blipbox.eventhandler == &blipbox.midizones)
    preset = 0;
  else
    preset = blipbox.midizones.getPreset()+1;
  printPreset();
}

void PresetChooser::printPreset(){
  if(preset)
    blipbox.display.printCharacter('0'+preset, 3, 0, blipbox.config.brightness);
  else
    blipbox.display.printCharacter('u', 3, 0, blipbox.config.brightness);
  // draw arrows left/right
  if(preset){
    blipbox.leds.setLed(1, 3, blipbox.config.brightness);
    blipbox.leds.setLed(0, 4, blipbox.config.brightness);
    blipbox.leds.setLed(1, 5, blipbox.config.brightness);
  }
  if(preset < MAX_PRESET){
    blipbox.leds.setLed(8, 3, blipbox.config.brightness);
    blipbox.leds.setLed(9, 4, blipbox.config.brightness);
    blipbox.leds.setLed(8, 5, blipbox.config.brightness);
  }
  // draw marker for bottom left corner
  blipbox.leds.setLed(1, 0, blipbox.config.brightness);
  blipbox.leds.setLed(0, 1, blipbox.config.brightness);
}

void PresetChooser::tick(uint16_t counter){
  uint8_t brightness = ((counter >> 6) % blipbox.config.brightness);
  blipbox.leds.setLed(0, 0, blipbox.config.brightness-brightness);
}

uint8_t column, origin;
void PresetChooser::press(Position& pos){
  origin = pos.column;
  column = pos.column;
}

void PresetChooser::drag(Position& pos){
  if(pos.row < 2)
    return;
  if(pos.column > column && preset < MAX_PRESET)
    blipbox.display.shift(SHIFT_RIGHT);
  else if(pos.column < column && preset)
    blipbox.display.shift(SHIFT_LEFT);
  column = pos.column;
}

void PresetChooser::release(Position& pos){
  if(pos.column > origin && preset)
    preset--;
  else if(pos.column < origin && preset < MAX_PRESET)
    preset++;
  blipbox.leds.clear();
  printPreset();
}

void PresetChooser::taptap(Position& pos){
  if(pos.column < 2 && pos.row < 2){
    blipbox.setEditMode(false); // changes the event handler
    if(preset){
      // load preset, set midi zone mode
      blipbox.midizones.loadPreset(preset-1);
      blipbox.setMidiMode(true);
    }else{
      // set usb mode
      blipbox.setMidiMode(false);
    }
  }else if(pos.row > 1 && pos.column > 1 && pos.column < 8 && preset){
    // save preset to eeprom
    blipbox.midizones.savePreset(preset-1);
    blipbox.message(ALERT);
  }
}
