#include <stdint.h>
#include "globals.h"
#include "PresetChooser.h"

void PresetChooser::printPreset(){
  if(preset)
 // ascii numbers start at hex 30
    blipbox.display.printCharacter(preset+0x30, 3, 0, blipbox.config.brightness);
  else
    blipbox.display.printCharacter('u', 3, 0, blipbox.config.brightness);
}

void PresetChooser::init(){
  blipbox.setFollowMode(0);
  if(blipbox.eventhandler != &blipbox.midizones)
    preset = blipbox.midizones.getPreset()+1;
  printPreset();
}

void PresetChooser::tick(uint16_t counter){
  // draw animated arrows left/right
  uint8_t brightness = ((counter >> 6) % 100) + 100;
  if(preset){
    blipbox.leds.setLed(1, 3, brightness);
    blipbox.leds.setLed(0, 4, brightness+55);
    blipbox.leds.setLed(1, 5, brightness);
  }
  if(preset < 5){
    blipbox.leds.setLed(8, 3, brightness);
    blipbox.leds.setLed(9, 4, brightness+55);
    blipbox.leds.setLed(8, 5, brightness);
  }
  // draw marker for bottom left corner
  blipbox.leds.setLed(1, 0, 100);
  blipbox.leds.setLed(0, 0, 0xff-brightness);
  blipbox.leds.setLed(0, 1, 100);
}

void PresetChooser::tap(Position& pos){
  if(pos.row < 2)
    return;
  if(pos.column < 5){
    if(preset)
      --preset;
    printPreset();
  }else{
    if(preset<5)
      ++preset;
    printPreset();
  }
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
  }
}
