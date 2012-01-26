#ifndef _PRESETREADER_H_
#define _PRESETREADER_H_

#include "SerialReader.h"
#include "Preset.h"
#include "defs.h"
#include <inttypes.h>

class PresetReader : public SerialReader {
private:
  Preset* preset;
  uint8_t index;
  uint8_t command;
  uint8_t checksum;
  uint8_t pos;
  uint8_t buf[MIDI_ZONES_IN_PRESET*MIDI_ZONE_PRESET_SIZE];
public:
  PresetReader(Preset* preset);
  void serialInput(unsigned char c);
};

#endif /* _PRESETREADER_H_ */

