#include "device.h"
#include "MidiInterface.h"
#include "MidiReader.h"
#include "MidiWriter.h"
#include "TouchController.h"

MidiReader reader;
MidiWriter writer;

#define SENSOR_MAX          1023
#define TOUCH_THRESH        500
#define FSR_THRESH          8

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

#define OP1     1
#define OP2     2
#define OP3     3
uint8_t operation;

#define CC_MODULATION    0
#define CC_VOLUME        1

uint8_t CC_CODES_SET1[] = { 1, 2 };
uint8_t CC_CODES_SET2[] = { 1, 2 };
uint8_t CC_CODES_SET3[] = { 1, 2 };

#define BUTTON1_PIN PD2
#define BUTTON2_PIN PD4
#define BUTTON_DDR  DDRD
#define BUTTON_PORT PORTD
#define BUTTON_PINS PIND

#define BUTTON_PINMAP (_BV(BUTTON1_PIN) | _BV(BUTTON2_PIN))

uint8_t ccvalues[2];
uint8_t *cccodes;

uint8_t buttons;

class Event {
public:
  boolean trigger;
  uint8_t note;
  uint8_t modulation;
  uint8_t volume;
  uint16_t pitchbend;
};

Event event;
uint16_t pitchbend;

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

uint8_t getStringPitch(){
  string = getString();
//   writer.controlChange(10, string);
  return (pot*note_range)/1023 + string*5 + note_root;
}

uint8_t getStripPitch(){
  return (pot*note_range)/1023 + note_root;
}

void noteOff(){
  if(note != -1){
    writer.noteOff(note, 0);
    note = -1;
  }
}

void noteOn(){
  noteOff();
  writer.noteOn(event.note, event.volume);
  note = event.note;
}

void updateController(uint8_t cc, uint8_t value){
  if(cccodes[cc] && ccvalues[cc] != value){
    writer.controlChange(cccodes[cc], value);
    ccvalues[cc] = value;
  }
}

void updateSettings(){
  buttons = BUTTON_PINS & BUTTON_PINMAP;
  if(buttons & _BV(BUTTON1_PIN)){
    operation = OP1;
    cccodes = CC_CODES_SET1;
  }else if(buttons & _BV(BUTTON2_PIN)){
    operation = OP2;
    cccodes = CC_CODES_SET3;
  }else{
    operation = OP3;
    cccodes = CC_CODES_SET2;
  }
  noteOff();
  writer.allNotesOff();
}

/*
modes:
trigger: fsr, pad z.
modulation: fsr, pad x
trigger on pad z, notes on strip and pad y, mod on fsr
trigger on fsr, notes
no trigger, pb on strip, volume (cc2) on pad y
pitch bend on strip

mode    trigger    note        modulation   volume
OP1     pad z      strip+pad   pad x        fsr
OP3     pad z      0           pad x        pad y
OP2     fsr        strip       fsr          pad x

*/

void updateEvent(){
  switch(operation){
  case OP1:
    event.trigger = screen.getZ() < TOUCH_THRESH;
    event.modulation = screen.getX() >> 3;
    event.volume = screen.getValue(7) >> 3;
    event.note = getStringPitch();
    event.pitchbend = 8192;
    break;
  case OP2:
    event.trigger = screen.getValue(7) > FSR_THRESH;
    event.modulation = screen.getValue(7) >> 3;
    event.volume = screen.getX() >> 3;
    event.note = getStripPitch();
    event.pitchbend = 8192;
    break;
  case OP3:
    event.trigger = screen.getZ() < TOUCH_THRESH;
    event.modulation = screen.getX() >> 3;
    event.volume = screen.getY() >> 3;
    event.note = 0;
    event.pitchbend = pot << 4;
    break;
  }
}

void loop(){
  if((BUTTON_PINS & BUTTON_PINMAP) != buttons)
    updateSettings();

  if(screen.getValue(6) > 1)
    pot = SENSOR_MAX - screen.getValue(6);

  if(screen.getZ() < TOUCH_THRESH){
    x = screen.getX();
//     y = SENSOR_MAX - screen.getY();
    y = screen.getY();
  }else{
    x = 0;
    y = 0;
  }

  updateEvent();

  if(event.trigger){
    if(state == STANDBY_STATE){
      noteOn();
      state = SUSTAIN_STATE;
    }else if(note != event.note){
      noteOn();
    }
    updateController(CC_VOLUME, event.volume);
    updateController(CC_MODULATION, event.modulation);
    if(event.pitchbend != pitchbend){
      writer.pitchBend(event.pitchbend);
      pitchbend = event.pitchbend;
    }
  }else if(state != STANDBY_STATE){
    noteOff();
    state = STANDBY;
  }
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
