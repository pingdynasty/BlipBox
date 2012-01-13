#include "Preset.h"
#include "Event.h"

#include "defs.h"
#include "globals.h"
#include <stddef.h>
#include <string.h>
#include <avr/eeprom.h>

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
    uint8_t bts = zones[i].read(buf);
    offset += bts;
//     offset += MIDI_ZONE_PRESET_SIZE;
  }
}

void Preset::save(uint8_t index){
  uint8_t buf[MIDI_ZONE_PRESET_SIZE];
  uint16_t offset = getAddress(index);
  for(uint8_t i=0; i<MIDI_ZONES_IN_PRESET; ++i){
    memset(buf, 0, sizeof(buf));
    uint8_t bts = zones[i].write(buf);
    eeprom_write_block(buf, (uint8_t*)offset, bts);
    offset += bts;
//     offset += MIDI_ZONE_PRESET_SIZE;
  }
}

void Preset::handle(MidiEvent& event){
  for(int i=0; i<MAX_ZONES_IN_PRESET; ++i){
    // todo: remove this check when zones are guaranteed to have actions
//     if(zones[i].action != NULL && 
//        zones[i].action->getType() == event.getStatus())
    if(zones[i].action != NULL)
      zones[i].action->handle(event);
  }
}

void Preset::handle(TouchEvent& event){
  for(int i=0; i<MAX_ZONES_IN_PRESET; ++i)
    if(event.getPosition() == NULL || zones[i].match(event.getPosition()))
      // release events have NULL position
      // todo: remove this check when zones are guaranteed to have actions
      if(zones[i].action != NULL)
	zones[i].handle(event);
  if(event.isTapTap() && event.getPosition()->getColumn() == 0 && event.getPosition()->getRow() == 0)
    blipbox.setMode(EDIT_MODE);
}

void Preset::tick(uint16_t counter){
  blipbox.leds.bitshiftright();
  for(int i=0; i<MAX_ZONES_IN_PRESET; ++i){
    // todo: remove this check when zones are guaranteed to have actions
    if(zones[i].action != NULL){
      switch(zones[i].getDisplayType()){
      case FILL_DISPLAY_TYPE:
	zones[i].fill();
	break;
      case LINE_DISPLAY_TYPE:
	zones[i].line();
	break;
//       case NONE_DISPLAY_TYPE:
//       default:
      }
    }
  }
  blipbox.leds.flip();
}
