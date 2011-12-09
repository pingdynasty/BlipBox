#include "MidiPresetReader.h"
#include <avr/eeprom.h>
#include "globals.h"

#define RECEIVE_PRESET_COMMAND 1
#define REQUEST_PRESET_COMMAND 2
#define MIDI_ZONE_PRESET_LENGTH 1+MIDI_ZONES_IN_PRESET*MIDI_ZONE_PRESET_SIZE+1

void MidiPresetReader::serialInput(unsigned char c){
  blipbox.leds.setLed(index % 10, index / 10, blipbox.config.brightness);
  if(++index == 1){
    command = c >> 4;
    preset = c & 0x0f;
  }else if(command == REQUEST_PRESET_COMMAND){
    blipbox.resetSerialReader();
    if(c == 0){
      blipbox.loadPreset(preset);
      blipbox.sendPreset();
      blipbox.message(ALERT);
    }else{
      blipbox.message(MESSAGE_READ_ERROR);
    }
  }else if(command == RECEIVE_PRESET_COMMAND){
    if(index == MIDI_ZONE_PRESET_LENGTH){
      blipbox.resetSerialReader();
      if(c == checksum){
	eeprom_write_block(buf, (uint8_t*)(MIDI_PRESET_OFFSET+preset*sizeof(buf)), sizeof(buf));
	blipbox.message(ALERT);
      }else{
	blipbox.message(MESSAGE_READ_ERROR);
      }
    }else{
      buf[index-2] = c;
      // 	checksum ^= c;
    }
  }else{
    blipbox.resetSerialReader();
    blipbox.message(MESSAGE_READ_ERROR);
  }
}
