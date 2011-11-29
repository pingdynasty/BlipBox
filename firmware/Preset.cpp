#include "Preset.h"
#include "Event.h"

#include "defs.h"
#include <stddef.h>
#include <avr/eeprom.h>

void Preset::handle(Event& event){
  for(int i=0; i<MAX_ZONES_IN_PRESET; ++i)
    if(event.getPosition() == NULL || zones[i].match(event.getPosition()))
      zones[i].handle(event);
}

#define MIDI_PRESET_HEADER 0
void Preset::loadPreset(uint8_t index){
  uint16_t offset = MIDI_PRESET_OFFSET + MIDI_PRESET_HEADER +
    index * MIDI_ZONES_IN_PRESET * MIDI_ZONE_PRESET_SIZE;
  for(uint8_t i=0; i<MIDI_ZONES_IN_PRESET; ++i){
    uint8_t buf[MIDI_ZONE_PRESET_SIZE];
    eeprom_read_block(buf, (uint8_t*)offset, sizeof(buf));
    zones[index].read(buf);
    offset += MIDI_ZONE_PRESET_SIZE;
  }
}

void Preset::tick(uint16_t counter){
  for(int i=0; i<MAX_ZONES_IN_PRESET; ++i)
    zones[i].draw();
}
