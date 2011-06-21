#ifndef _MIDIPRESETREADER_H_
#define _MIDIPRESETREADER_H_

#include "SerialReader.h"

#define READ_PRESET_COMMAND    1
#define REQUEST_PRESET_COMMAND 2
#define PRESET_LENGTH 34 // 1 + 8*ZONE_LENGTH + 1 = 34
#define ZONE_LENGTH 4

#include <wiring.h>

class MidiPresetReader : public SerialReader {
public:
  uint8_t index;
  uint8_t zone;
  uint8_t command;
  unsigned char rx_buf[ZONE_LENGTH];
  MidiPresetReader() : index(0), zone(0) {}
  void serialInput(unsigned char c){
    blipbox.leds.setLed(index % 10, index / 10, blipbox.config.brightness);
    if(++index == 1){
      command = c >> 4;
      uint8_t preset = c & 0x0f;
      blipbox.midizones.setPreset(preset);
    }else if(command == REQUEST_PRESET_COMMAND){
      blipbox.resetSerialReader();
      blipbox.sendMidiZones();
      if(c != 0)
	blipbox.message(MESSAGE_READ_ERROR);
    }else if(command == READ_PRESET_COMMAND){
      if(index == PRESET_LENGTH){
	blipbox.resetSerialReader();
	if(c != 0 || zone != 8)
	  blipbox.message(MESSAGE_READ_ERROR);
	else
	  blipbox.setMidiMode(true);
      }else{
	uint8_t i = (index-2) % ZONE_LENGTH;
	rx_buf[i] = c;
	if(i == ZONE_LENGTH-1)
	  blipbox.midizones.getZone(zone++).read(rx_buf);
      }
    }else{
      blipbox.resetSerialReader();
      blipbox.message(MESSAGE_READ_ERROR);
    }
  }
};

#endif /* _MIDIPRESETREADER_H_ */

