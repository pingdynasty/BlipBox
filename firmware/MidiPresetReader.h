#ifndef _MIDIPRESETREADER_H_
#define _MIDIPRESETREADER_H_

#include "SerialReader.h"

#define READ_PRESET_COMMAND 1
#define PRESET_LENGTH 34 // 1 + 8*4 + 1 = 34

class MidiPresetReader : public SerialReader {
  uint8_t index;
  uint8_t zone;
  uint8_t command;
  unsigned char rx_buf[];
  void serialInput(unsigned char c){
    if(++index == 1){
      command = c >> 4;
      uint8_t preset = c & 0x0f;
      blipbox.midizones.setPreset(preset);
    }else if(index == PRESET_LENGTH){
      if(c != 0 || zone != 8)
	blipbox.message(MESSAGE_READ_ERROR);
      blipbox.resetSerialReader();
    }else if(command == READ_PRESET_COMMAND){
      uint8_t i = index-2 % 3;
      rx_buf[i] = c;
      if(i == 2)
	blipbox.midizones.getZone(zone++).read(rx_buf);
    }else{
      blipbox.message(MESSAGE_READ_ERROR);
      blipbox.resetSerialReader();
    }
  }
};

#endif /* _MIDIPRESETREADER_H_ */

