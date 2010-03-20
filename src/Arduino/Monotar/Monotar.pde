#include "device.h"
#include "MidiInterface.h"
#include "MidiReader.h"
#include "MidiWriter.h"
#include "TouchController.h"

MidiReader reader;
MidiWriter writer;

#define SENSOR_MAX          1023
#define TOUCH_THRESH        500
#define VELOCITY            80

class CommandInterface : public MidiInterface {
public:
};

CommandInterface command;
TouchController screen;

#define STANDBY_STATE   0
#define SUSTAIN_STATE   1

uint16_t x, y, pot;
int8_t note;
uint8_t state;
uint8_t string;
uint8_t note_root, note_range;

#define CC_X    0
#define CC_Y    1
#define CC_Z    2
#define CC_POT1 3
#define CC_POT2 4

// #define CC_CODES_SET1 { 0, 0, 0, 1, 2 }
// #define CC_CODES_SET2 { 0, 0, 1, 2, 0 }
// #define CC_CODES_SET3 { 0, 0, 2, 1, 0 }

// uint8_t CC_CODES_SET1[] = { 1, 2, 3, 4, 5 };
uint8_t CC_CODES_SET1[] = { 2, 0, 0, 0 ,1 };
uint8_t CC_CODES_SET2[] = { 2, 0, 1, 0, 0 };
uint8_t CC_CODES_SET3[] = { 1, 0, 2, 0, 0 };

#define BUTTON1_PIN PD2
#define BUTTON2_PIN PD4
#define BUTTON_DDR  DDRD
#define BUTTON_PORT PORTD
#define BUTTON_PINS PIND

#define BUTTON_PINMAP (_BV(BUTTON1_PIN) | _BV(BUTTON2_PIN))

uint8_t ccvalues[6];
uint8_t *cccodes;

uint8_t buttons;

void setup(){
  note_root = 36;
  note_range = 24;
  note = -1;
//   cccodes = CC_CODES_SET1;

  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);
  BUTTON_DDR &= ~_BV(BUTTON1_PIN);
  BUTTON_DDR &= ~_BV(BUTTON2_PIN);
//   pinMode(BUTTON1_PIN, INPUT);
//   pinMode(BUTTON2_PIN, INPUT);

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
  return (y*6) / 1023 - 1;
}

uint8_t getPitch(){
  string = getString();
//   writer.controlChange(10, string);
  return (pot*note_range)/1023 + string*5 + note_root;
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
  writer.noteOn(note, VELOCITY);
}

void updateController(uint8_t cc, uint8_t value){
  if(cccodes[cc] && ccvalues[cc] != value){
    writer.controlChange(cccodes[cc], value);
    ccvalues[cc] = value;
  }
}

void updateSettings(){
  buttons = BUTTON_PINS & BUTTON_PINMAP;
  if(buttons & _BV(BUTTON1_PIN))
    cccodes = CC_CODES_SET1;
  else if(buttons & _BV(BUTTON2_PIN))
    cccodes = CC_CODES_SET3;
  else
    cccodes = CC_CODES_SET2;

  writer.allNotesOff();
}

void loop(){
  if((BUTTON_PINS & BUTTON_PINMAP) != buttons)
    updateSettings();

  pot = SENSOR_MAX - screen.getValue(6);
  if(pot > 1021)
    pot = 0;
  if(screen.getZ() < TOUCH_THRESH){
    x = screen.getX();
    y = SENSOR_MAX - screen.getY();
    switch(state){
    case STANDBY_STATE:
      noteOn();
      state = SUSTAIN_STATE;
      break;
    case SUSTAIN_STATE:
      if(note != getPitch())
        noteOn();
      break;
    }
    // truncate CC values to 7 bits
    updateController(CC_X, x>>3);
    updateController(CC_Y, y>>3);
    updateController(CC_Z, screen.getZ()>>3);
  }else if(state != STANDBY_STATE){
    noteOff();
    updateController(CC_X, 0);
    updateController(CC_Y, 0);
    updateController(CC_Z, 0);
    state = STANDBY;
  }
  updateController(CC_POT1, pot>>3);
  updateController(CC_POT2, screen.getValue(7)>>3);

//   delay(10);
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
