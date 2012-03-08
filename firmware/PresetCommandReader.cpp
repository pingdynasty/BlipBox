#include "PresetCommandReader.h"
#include "Command.h"
#include "globals.h"

#define MIDI_ZONE_PRESET_LENGTH 1+MIDI_ZONES_IN_PRESET*MIDI_ZONE_PRESET_SIZE+1

#define NO_COMMAND -1
PresetCommandReader::PresetCommandReader() : command(NO_COMMAND) {}

void PresetCommandReader::serialInput(unsigned char c){
  if(command == NO_COMMAND){
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
    blipbox.receivePreset(index);
  }else{
    blipbox.resetSerialReader();
    blipbox.message(MESSAGE_READ_ERROR);
  }
}
