
#include <stdint.h>
#include "globals.h"
#include "EventHandler.h"

void PresetChooser::printPreset(){
  if(preset)
 // ascii numbers start at hex 30
    blipbox.display.printCharacter(preset+0x30, 3, 0, blipbox.config.brightness);
  else
    blipbox.display.printCharacter('u', 3, 0, blipbox.config.brightness);
}

void PresetChooser::init(){
  blipbox.setFollowMode(0);
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
  if(blipbox.keys.getRow() < 2)
    return;
  if(blipbox.keys.getColumn() < 5){
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
  if(blipbox.keys.getColumn() < 2 && blipbox.keys.getRow() < 2){
    blipbox.setEditMode(false); // changes the event handler
    if(preset){
      // load preset, set midi zone mode
      blipbox.sender.midiprotocol.loadPreset(preset-1);
      blipbox.setMidiMode(true);
//       blipbox.setEventHandler(new MidiZonesEventHandler());
    }else{
      // set usb mode
      blipbox.setMidiMode(false);
    }
  }
}

void DefaultEventHandler::press(Position& pos){
  blipbox.sender.position.update(blipbox.keys.getPosition().x, blipbox.keys.getPosition().y);
  blipbox.sender.release.reset();
}

void DefaultEventHandler::release(Position& pos){
  blipbox.sender.position.reset();
  blipbox.sender.release.update();
}

void DefaultEventHandler::drag(Position& pos){
  blipbox.sender.position.update(blipbox.keys.getPosition().x, blipbox.keys.getPosition().y);
}

void DefaultEventHandler::taptap(Position& pos){
  if(blipbox.keys.getColumn() == 0 && blipbox.keys.getRow() == 0){
    blipbox.setEditMode(true);
  }
}
