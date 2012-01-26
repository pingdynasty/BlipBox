#include "Preset.h"
#include "Event.h"

#include "defs.h"
#include "globals.h"
#include <stddef.h>
#include <string.h>
#include <avr/eeprom.h>

Preset::Preset(){
  memset(zones, 0, sizeof(zones));
}

#define MIDI_PRESET_HEADER 0
uint16_t getAddress(uint8_t index){
  return MIDI_PRESET_OFFSET + MIDI_PRESET_HEADER +
    index * MIDI_ZONES_IN_PRESET * MIDI_ZONE_PRESET_SIZE;
}

void Preset::load(uint8_t index){
  uint8_t buf[MIDI_ZONE_PRESET_SIZE];
  uint16_t offset = getAddress(index);
  for(uint8_t i=0; i<MIDI_ZONES_IN_PRESET; ++i){
    memset(buf, 0, sizeof(buf));
    eeprom_read_block(buf, (uint8_t*)offset, sizeof(buf));
//     offset += zones[i].read(buf);
    offset += readZone(buf, i);
  }
}

void Preset::save(uint8_t index){
  uint8_t buf[MIDI_ZONE_PRESET_SIZE];
  uint16_t offset = getAddress(index);
  for(uint8_t i=0; i<MIDI_ZONES_IN_PRESET; ++i){
    uint8_t bts = writeZone(buf, i);
    eeprom_write_block(buf, (uint8_t*)offset, bts);
    offset += bts;
  }
}

void Preset::handle(MidiEvent& event){
  for(int i=0; i<MAX_ZONES_IN_PRESET; ++i){
    // todo: remove this check when zones are guaranteed to have actions
    if(zones[i] != NULL && zones[i]->action != NULL)
      zones[i]->action->handle(event);
  }
}

void Preset::handle(TouchEvent& event){
  for(int i=0; i<MAX_ZONES_IN_PRESET; ++i)
    if(zones[i] != NULL && (event.getPosition() == NULL || zones[i]->match(event.getPosition())))
      // release events have NULL position
      // todo: remove this check when zones are guaranteed to have actions
      if(zones[i]->action != NULL)
	zones[i]->handle(event);
  if(event.isTapTap() && event.getPosition()->getColumn() == 0 && event.getPosition()->getRow() == 0)
    blipbox.setMode(EDIT_MODE);
}

void Preset::tick(uint16_t counter){
  blipbox.leds.bitshiftright();
  for(int i=0; i<MAX_ZONES_IN_PRESET; ++i){
    // todo: remove action check when zones are guaranteed to have actions
    if(zones[i] != NULL && zones[i]->action != NULL){
      switch(zones[i]->getDisplayType()){
      case FILL_DISPLAY_TYPE:
	zones[i]->fill();
	break;
      case LINE_DISPLAY_TYPE:
	zones[i]->line();
	break;
//       case NONE_DISPLAY_TYPE:
//       default:
      }
    }
  }
  blipbox.leds.flip();
}

uint8_t Preset::readZone(const uint8_t* data, uint8_t index){
  delete zones[index];
  zones[index] = Zone::createZone(data[0]);
  return zones[index]->read(data);
}

uint8_t Preset::writeZone(uint8_t* data, uint8_t index){
  if(zones[index] == NULL)
    return 0;
  return zones[index]->write(data);
}
