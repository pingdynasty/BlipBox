#include <stdint.h>
#include "globals.h"
#include "PresetChooser.h"
#include "defs.h"

void PresetChooser::init(){
  if(blipbox.eventhandler == &blipbox.midizones)
    preset = 0;
  else
    preset = blipbox.midizones.preset+1;
  printPreset();
}

void PresetChooser::printPreset(){
  if(preset)
    blipbox.display.printCharacter('0'+preset, 3, 0, blipbox.config.brightness);
  else
    blipbox.display.printCharacter('U', 3, 0, blipbox.config.brightness);
  // draw arrows left/right
  if(preset){
    blipbox.leds.setLed(1, 3, blipbox.config.brightness);
    blipbox.leds.setLed(0, 4, blipbox.config.brightness);
    blipbox.leds.setLed(1, 5, blipbox.config.brightness);
  }
  if(preset < MIDI_ZONE_PRESETS){
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
  blipbox.leds.setLed(1, 0, 0);
  blipbox.leds.setLed(0, 1, 0);
  blipbox.leds.setLed(0, 0, 0);
  if(pos.column > column && preset)
    blipbox.display.shift(SHIFT_RIGHT);
  else if(pos.column < column && preset < MIDI_ZONE_PRESETS)
    blipbox.display.shift(SHIFT_LEFT);
  column = pos.column;
}

void PresetChooser::release(Position& pos){
  if(pos.column > origin && preset)
    preset--;
  else if(pos.column < origin && preset < MIDI_ZONE_PRESETS)
    preset++;
  blipbox.leds.clear();
  printPreset();
}

void PresetChooser::taptap(Position& pos){
  if(pos.column < 2 && pos.row < 2){
    blipbox.setEditMode(false);
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
