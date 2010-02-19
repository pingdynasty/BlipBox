#include "MidiInterface.h"
#include "MidiReader.h"
#include "MidiWriter.h"
#include "Sequencer.h"
#include "TouchController.h"

uint16_t touchscreen_x_min   = 250;
uint16_t touchscreen_x_range = 810 - touchscreen_x_min;
uint16_t touchscreen_y_min   = 210;
uint16_t touchscreen_y_range = 784 - touchscreen_y_min;

MidiReader reader;
MidiWriter writer;
// ConsoleReader reader;
// ConsoleWriter writer;

#define CC_X         2
#define CC_Y         3
#define CC_Z         6
#define CC_POT       7
#define CC_DX        4
#define CC_DY        5

#define TOUCH_THRESH        300
#define STRUMMING_THRESHOLD 20

class CommandInterface : public MidiInterface {
public:
};

CommandInterface command;
TouchController screen;

#define STANDBY_STATE   0
#define ACTIVATED_STATE 1
#define STRUMMING_STATE 2
#define SUSTAIN_STATE   3

// uint16_t tick;
uint16_t x, y, pot;
int8_t dx, dy;
int8_t note;
uint8_t state;
uint8_t string;
uint8_t note_root, note_range;

void setup(){

  note_root = 36;
  note_range = 24;

  cli(); // disable interrupts

  screen.init();
  writer.init(1); // write output to MIDI channel 1
  reader.init(&command);

  sei(); // enable interrupts

//   Serial.begin(9600);
  beginSerial(38400);
//     beginSerial(31250);
}

uint8_t getString(){
  return (y * 6) / 1023 + 1;
}

uint8_t getPitch(){
  string = getString();
  writer.controlChange(10, string);
  return (pot*note_range)/1023 + string*5 + note_root;
}

uint8_t getVelocity(){
  return dy * 10;
}

void noteOff(){
  if(note > 0){
    writer.noteOff(note, 0);
    note = -1;
  }
}

void noteOn(){
  noteOff();
  note = getPitch();
  writer.noteOn(note, getVelocity());
}

void loop(){
  if(screen.getZ() < TOUCH_THRESH){
    // dx/dy valid iff there's a valid previous reading, i.e. state != STANDBY
    dx = x - screen.getX();
    dy = y - screen.getY();
    x = screen.getX();
    y = screen.getY();
    pot = screen.getValue(6);
    switch(state){
    case STANDBY_STATE:
      state = ACTIVATED_STATE;
      break;
    case ACTIVATED_STATE:
      noteOn();
//       if(dy > STRUMMING_THRESHOLD)
//         state = STRUMMING_STATE;
//       else
        state = SUSTAIN_STATE;
      break;
    case STRUMMING_STATE:
      noteOff();
      // fall through
    case SUSTAIN_STATE:
//       if(string != getString())
      if(note != getPitch())
        noteOn();
      break;
    }
    // truncate CC values to 7 bits
    writer.controlChange(CC_X, x >> 3);
    writer.controlChange(CC_Y, y >> 3);
    writer.controlChange(CC_Z, screen.getZ() >> 2);
    writer.controlChange(CC_POT, pot >> 3);
    writer.controlChange(CC_DX, dx >> 3);
    writer.controlChange(CC_DY, dx >> 3);
    writer.controlChange(1, screen.check() >> 3);
  }else if(state != STANDBY_STATE){
    noteOff();
    writer.controlChange(CC_X, 0);
    writer.controlChange(CC_Y, 0);
    writer.controlChange(CC_Z, 0);
    state = STANDBY;
  }

//   if(screen.check() < TOUCH_THRESH){
//     for(int i=0; i<6; ++i)
//       writer.controlChange(i+1, screen.getValue(i));
//   }
  delay(120);
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
