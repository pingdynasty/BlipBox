#include "MidiReader.h"
#include "MidiWriter.h"
#include "Sequencer.h"
#include "ConsoleReader.h"
#include "ConsoleWriter.h"
#include "MidiCvInterface.h"

// PWM cv outputs square pulse wave (at the 15kHz sampling rate probably)
// vactrol? 100uF cap to ground seems to cut the noise.

MidiCvInterface cv;
MidiReader reader;
MidiWriter writer;
// ConsoleReader reader;
// ConsoleWriter writer;
Sequencer seq;

void setup(){
  // MIDI in sends controls to sequencer, 
  // which sends to MIDI out for controller feedback 
  // and CV out for playback.
  cv.init();
  writer.init(1);
  seq.init(&cv, &writer);
  reader.init(&seq);
}

uint16_t counter;
void loop() {
  if(Serial.available())
    reader.read();
  if(counter++ == 512){
    seq.midiClock();
    counter = 0;
  }
}
