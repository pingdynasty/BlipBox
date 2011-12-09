#ifndef _MIDIPRESETREADER_H_
#define _MIDIPRESETREADER_H_

#include "SerialReader.h"
#include "defs.h"
#include <inttypes.h>

class MidiPresetReader : public SerialReader {
public:
  uint8_t index;
  uint8_t preset;
  uint8_t command;
  unsigned char checksum;
  uint8_t buf[MIDI_ZONES_IN_PRESET*MIDI_ZONE_PRESET_SIZE];
  MidiPresetReader() : index(0), checksum(0) {}
  void serialInput(unsigned char c);
};

#endif /* _MIDIPRESETREADER_H_ */

