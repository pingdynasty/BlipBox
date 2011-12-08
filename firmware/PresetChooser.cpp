#include <stdint.h>
#include "globals.h"
#include "PresetChooser.h"
#include "defs.h"

void PresetChooser::init(){
  preset = blipbox.config.preset;
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
//   // draw marker for bottom left corner
//   blipbox.leds.setLed(1, 0, blipbox.config.brightness);
//   blipbox.leds.setLed(0, 1, blipbox.config.brightness);
}

void PresetChooser::press(Position& pos){
  origin = pos.getColumn();
  column = pos.getColumn();
}

void PresetChooser::drag(Position& pos){
  if(pos.getRow() < 2)
    return;
  blipbox.leds.setLed(1, 0, 0);
  blipbox.leds.setLed(0, 1, 0);
  blipbox.leds.setLed(0, 0, 0);
  if(pos.getColumn() > column && preset)
    blipbox.display.shift(SHIFT_RIGHT);
  else if(pos.getColumn() < column && preset < MIDI_ZONE_PRESETS)
    blipbox.display.shift(SHIFT_LEFT);
  column = pos.getColumn();
}

void PresetChooser::release(Position& pos){
  if(pos.getColumn() > origin && preset)
    preset--;
  else if(pos.getColumn() < origin && preset < MIDI_ZONE_PRESETS)
    preset++;
  blipbox.leds.clear();
  printPreset();
}

void PresetChooser::taptap(Position& pos){
  Coordinate ref(2, 2);
  if(pos < ref){
    blipbox.setEditMode(false);
    blipbox.loadPreset(preset);
  }
}

void PresetChooser::tick(uint16_t counter){
  blipbox.leds.setLed(0, 0, blipbox.leds.getLed(0, 0)-6);
  blipbox.leds.flip();
}
