#include "MidiPresetReader.h"
#include <avr/eeprom.h>
#include "Command.h"
#include "globals.h"

#define MIDI_ZONE_PRESET_LENGTH 1+MIDI_ZONES_IN_PRESET*MIDI_ZONE_PRESET_SIZE+1

void MidiPresetReader::serialInput(unsigned char c){
  if(index++ == 0){
    command = c & 0xf0;
    preset = c & 0x0f;
  }else if(command == PRESET_REQUEST_COMMAND){
    blipbox.resetSerialReader();
    if(c == 0){
      blipbox.sendPreset(preset);
      blipbox.message(ALERT);
    }else{
      blipbox.message(MESSAGE_READ_ERROR);
    }
  }else if(command == PRESET_RECEIVE_COMMAND){
    if(index == MIDI_ZONE_PRESET_LENGTH){
      blipbox.leds.clear();
      blipbox.resetSerialReader();
      if(c == checksum){
	eeprom_write_block(buf, (uint8_t*)(MIDI_PRESET_OFFSET+preset*sizeof(buf)), sizeof(buf));
	blipbox.message(ALERT);
      }else{
	blipbox.message(MESSAGE_READ_ERROR);
      }
    }else{
      buf[index-2] = c;
      blipbox.leds.setLed(index % 10, index / 10, blipbox.config.brightness);
      // 	checksum ^= c;
    }
  }else{
    blipbox.resetSerialReader();
    blipbox.message(MESSAGE_READ_ERROR);
  }
}
