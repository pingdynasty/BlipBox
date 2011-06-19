#ifndef _MIDIPROTOCOL_H_
#define _MIDIPROTOCOL_H_

#include <inttypes.h>
#include "Protocol.h"

#define MIDI_CC_X     26
#define MIDI_CC_Y     27

class MidiProtocol : public Protocol {
private:
  uint8_t channel;
  void controlChange(uint8_t cc, uint8_t value){
    serialWrite(0xb0 | channel);
    serialWrite(cc & 0x7f);
    serialWrite(value & 0x7f);
  }

public:
//   void sendParameterMessage(uint8_t pid, uint16_t value){
//     // todo!
//   }
//   void sendSensorMessage(uint8_t sid, uint16_t value){
//   }
//   void sendReleaseMessage(){
//   }
  void sendPositionMessage(uint16_t x, uint16_t y){
    controlChange(MIDI_CC_X, x>>3);
    controlChange(MIDI_CC_Y, y>>3);
  }
};


#endif /* _MIDIPROTOCOL_H_ */
