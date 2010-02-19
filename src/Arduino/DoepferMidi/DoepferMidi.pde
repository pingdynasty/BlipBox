#include "MidiReader.h"
#include "MidiWriter.h"
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
//  seq.init(&cv, &writer);
//  reader.init(&seq);
  // bypass sequencer, send MIDI input straight to CV output.
   reader.init(&cv);

//   Serial.begin(9600);
  beginSerial(38400);
//     beginSerial(31250);
}

void loop() {
  seq.midiTick();
}

/* Serial RX interrupt */
#if defined(__AVR_ATmega168__)
SIGNAL(SIG_USART_RECV)
#else
SIGNAL(SIG_UART_RECV)
#endif
{
#if defined(__AVR_ATmega168__)
  unsigned char c = UDR0;
#else
  unsigned char c = UDR;
#endif
  reader.read(c);
}
