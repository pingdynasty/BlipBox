#ifndef _MIDICVINTERFACE_H_
#define _MIDICVINTERFACE_H_

#include <stdint.h>
#include "MCP492xController.h"
#include "MidiInterface.h"

class MidiCvInterface : public MidiInterface {

public:
  void init();
  void channelPressure(int value);
  void startSong();
  void stopSong();
  void midiClock();
  void pitchBend(int16_t value);
  void controlChange(int cc, int value);
  void noteOff(int note, int velocity);
  void noteOn(int note, int velocity);
  void allNotesOff();

  void setFactor(uint8_t _factor){
    factor = _factor;
  }

  uint8_t getFactor(){
    return factor;
  }

  void setCV(uint8_t cv, uint16_t value);
  uint16_t getCV(uint8_t cv);

/* private: */

  uint16_t note_to_cv(uint8_t note);
  int16_t pitchbend_to_cv(uint16_t pb);

  uint16_t cv1, cv2, cv3;
  uint8_t factor;
  uint8_t clock_divider;
  int keydowns;
  bool staccato;

  MCP492xController dac1;
  MCP492xController dac2;
  MCP492xController dac3;

};

#endif /* _MIDICVINTERFACE_H_ */
