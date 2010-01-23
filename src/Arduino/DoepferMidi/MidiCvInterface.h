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

  void setCV(int cv, uint8_t value);
  uint8_t getCV(int cv);

/* private: */

  uint16_t note_to_cv(uint8_t note);
  int16_t pitchbend_to_cv(uint16_t pb);

  uint16_t cv1;
  uint8_t factor;
  int keydowns;
  bool staccato;

  MCP492xController dac;

};

#endif /* _MIDICVINTERFACE_H_ */
