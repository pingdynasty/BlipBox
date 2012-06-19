#include "PresetCommandReader.h"
#include "Command.h"
#include "globals.h"

#define MIDI_ZONE_PRESET_LENGTH 1+MIDI_ZONES_IN_PRESET*MIDI_ZONE_PRESET_SIZE+1

#define NO_COMMAND -1

void PresetCommandReader::serialInput(unsigned char c){
  int8_t command = c & 0xf0;
  uint8_t index = c & 0x0f;
  if(command == PRESET_REQUEST_COMMAND){
    blipbox.sendPreset(index);
    blipbox.message(ALERT);
    blipbox.resetSerialReader(); // deletes this instance
  }else if(command == PRESET_RECEIVE_COMMAND){
    blipbox.receivePreset(index);
  }else{
    blipbox.message(MESSAGE_READ_ERROR);
    blipbox.resetSerialReader(); // deletes this instance
  }
}
