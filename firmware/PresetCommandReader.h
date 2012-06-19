#ifndef _PRESET_COMMAND_READER_H_
#define _PRESET_COMMAND_READER_H_

#include "SerialReader.h"
#include "Preset.h"
#include "defs.h"
#include <inttypes.h>

class PresetCommandReader : public SerialReader {
private:
/*   int8_t command; */
/*   uint8_t index; */
public:
/*   PresetCommandReader(); */
  void serialInput(unsigned char c);
};

#endif /* _PRESET_COMMAND_READER_H_ */

