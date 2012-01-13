#include <stdint.h>
#include "globals.h"
#include "PresetChooser.h"
#include "defs.h"

PresetChooser::PresetChooser(){
  preset = blipbox.getPresetIndex();
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

void PresetChooser::handle(TouchEvent& event){
  switch(event.getType()){
  case PRESS_EVENT_TYPE:
    press(*event.getPosition());
    break;
  case RELEASE_EVENT_TYPE:
    release();
    break;
  case TAPTAP_EVENT_TYPE:
    taptap(*event.getPosition());
    break;
  case DRAG_EVENT_TYPE:
    drag(*event.getPosition());
    break;
  }
}

void PresetChooser::press(Position& pos){
  origin = pos.getColumn();
  column = origin;
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

void PresetChooser::release(){
  if(column > origin && preset)
    preset--;
  else if(column < origin && preset < MIDI_ZONE_PRESETS)
    preset++;
  blipbox.leds.clear();
  printPreset();
}

void PresetChooser::taptap(Position& pos){
  Coordinate ref(2, 2);
  if(pos < ref)
    blipbox.loadPreset(preset);
}

void PresetChooser::tick(uint16_t counter){
  blipbox.leds.setLed(0, 0, blipbox.leds.getLed(0, 0)-6);
  blipbox.leds.flip();
}
