#include "MidiReader.h"
#include "MidiWriter.h"
#include "ConsoleReader.h"
#include "ConsoleWriter.h"
#include "MidiCvInterface.h"

/*

MCP4921 DAC
cs    - digital pin 2, PD2
SCK  - digital 13, PB5
SDI  - digital 11, PB3
VREF - vref
LDAC - gnd
AVSS - gnd
VCC  - +5v
Arduino pin 10, PB2 - vcc

cv2   - digital pin 5, PD5 (vactrol)
cv3   - digital pin 6, PD6 (lp filter)
gate1 - digital pin 7, PD7 (gate out)
gate2 - digital pin 8, PB0 (internal)

 */

// PWM cv outputs square pulse wave (at the 15kHz sampling rate probably)
// vactrol? 100uF cap to ground seems to cut the noise.

MidiCvInterface cv;
MidiReader reader;
MidiWriter writer;
// ConsoleReader reader;
// ConsoleWriter writer;

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
