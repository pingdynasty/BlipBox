#ifndef _MIDIPRESETREADER_H_
#define _MIDIPRESETREADER_H_

#include <avr/eeprom.h>
#include "defs.h"
#include "globals.h"
#include "SerialReader.h"

#define RECEIVE_PRESET_COMMAND 1
#define REQUEST_PRESET_COMMAND 2

#define MIDI_ZONE_PRESET_LENGTH 1+MIDI_ZONES_IN_PRESET*MIDI_ZONE_PRESET_SIZE+1

class MidiPresetReader : public SerialReader {
public:
  uint8_t index;
  uint8_t preset;
  uint8_t command;
  uint8_t buf[MIDI_ZONES_IN_PRESET*MIDI_ZONE_PRESET_SIZE];

  MidiPresetReader() : index(0) {}

  void serialInput(unsigned char c){
    blipbox.leds.setLed(index % 10, index / 10, blipbox.config.brightness);
    if(++index == 1){
      command = c >> 4;
      preset = c & 0x0f;
    }else if(command == REQUEST_PRESET_COMMAND){
      blipbox.resetSerialReader();
      if(c == 0){
	blipbox.midizones.loadPreset(preset);
	blipbox.sendMidiZones();
	blipbox.message(ALERT);
      }else{
	blipbox.message(MESSAGE_READ_ERROR);
      }
    }else if(command == RECEIVE_PRESET_COMMAND){
      if(index == MIDI_ZONE_PRESET_LENGTH){
	blipbox.resetSerialReader();
	if(c == 0){
	  eeprom_write_block(buf, (uint8_t*)(MIDI_PRESET_OFFSET+preset*sizeof(buf)), sizeof(buf));
	  blipbox.message(ALERT);
	}else{
	  blipbox.message(MESSAGE_READ_ERROR);
	}
      }else{
	buf[index-2] = c;
      }
    }else{
      blipbox.resetSerialReader();
      blipbox.message(MESSAGE_READ_ERROR);
    }
  }
};

#endif /* _MIDIPRESETREADER_H_ */

