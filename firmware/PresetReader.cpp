#include "PresetReader.h"
#include <avr/eeprom.h>
#include "Command.h"
#include "globals.h"

#define MIDI_ZONE_PRESET_LENGTH 1+MIDI_ZONES_IN_PRESET*MIDI_ZONE_PRESET_SIZE+1

PresetReader::PresetReader(Preset* p) : preset(p), pos(0), checksum(0) {}

void PresetReader::serialInput(unsigned char c){
  if(pos++ == 0){
    command = c & 0xf0;
    index = c & 0x0f;
  }else if(command == PRESET_REQUEST_COMMAND){
    blipbox.resetSerialReader();
    if(c == 0){
      blipbox.sendPreset(index);
      blipbox.message(ALERT);
    }else{
      blipbox.message(MESSAGE_READ_ERROR);
    }
  }else if(command == PRESET_RECEIVE_COMMAND){
    if(pos == MIDI_ZONE_PRESET_LENGTH){
      blipbox.leds.clear();
      blipbox.resetSerialReader();
      if(c == checksum){
	eeprom_write_block(buf, (uint8_t*)(MIDI_PRESET_OFFSET+index*sizeof(buf)), sizeof(buf));
	blipbox.message(ALERT);
      }else{
	blipbox.message(MESSAGE_READ_ERROR);
      }
    }else{
      buf[pos-2] = c;
      blipbox.leds.setLed(pos % 10, pos / 10, blipbox.config.brightness);
//       checksum ^= c;
    }
  }else{
    blipbox.resetSerialReader();
    blipbox.message(MESSAGE_READ_ERROR);
  }
}
