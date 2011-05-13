#include "MidiInterface.h"
#include "MidiReader.h"
#include "MidiWriter.h"
#include "Button.h"
#include "adc_freerunner.h"

MidiReader reader;
MidiWriter writer;

#define SERIAL_SPEED 9600
#define SERIAL_SPEED 31250

#define SENSOR_MAX          1023
#define CENTER_PITCHBEND 8192

#define AMP_PIN 5
#define MPX_PIN 4
#define POT_PIN 3
#define FSR_PIN 2
#define LAMP_PIN 10

#define MPX_THRESH 46 // at 40 caused the odd stray note
#define FSR_THRESH 10 // at 7 caused stray notes
#define POT_THRESH 8

// MPX5010DP pressure sensor, 5v single supply.

// 6/4 2011 fixed problems with second prototype (v2 + fsr):
// - bad solder joint on resonator pin
// - softpot pins 2/3 swapped: tap should be centre!

// MIDI CC values
// 1 Modulation (LSB 33)
// 2 Breath Controller (LSB 34)
// 7 Channel Volume
// 8 Balance
// 10 Pan
#define MIDI_MODULATION_CC  1
#define MIDI_BREATH_CC      2
#define MIDI_VOLUME_CC      7
#define MIDI_BALANCE_CC     8
#define MIDI_PAN_CC        10
#define MIDI_OSCMAINVOL_CC 36 // Access Virus B parameter
#define MIDI_NOTE_C2       36
#define MIDI_NOTE_C3       48
#define MIDI_NOTE_C4       60
#define MIDI_NOTE_A440     69
#define MIDI_NOTE_C5       72
#define MIDI_NOTE_C6       84

#define SERIAL_WRITE_INTERVAL 10L

unsigned long previousMillis = 0;        // will store last time write was done

OnOffOnButton button;

class CommandInterface : public MidiInterface {
public:
};

CommandInterface command;

#define STANDBY_STATE   0
#define SUSTAIN_STATE   1

int8_t note;
uint8_t state;
uint8_t string;
uint8_t note_root, note_range;

int16_t pot;
int16_t lastpot;

#define CNTRL_MODULATION    0
#define CNTRL_VOLUME        1

uint8_t CNTRL_CODES_SET[] = { MIDI_MODULATION_CC, MIDI_OSCMAINVOL_CC };

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
  note_root = MIDI_NOTE_C4;
  note_range = 24;
  note = -1;
  event.pitchbend = CENTER_PITCHBEND;
  cccodes = CNTRL_CODES_SET;
  cli(); // disable interrupts
  setup_adc();
  writer.init(1); // write output to MIDI channel 1
  reader.init(&command);
  button.init();
  pinMode(LAMP_PIN, OUTPUT);
  sei(); // enable interrupts
  beginSerial(SERIAL_SPEED);
  writer.allNotesOff();
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
  lastpot = pot;
}

void updateController(uint8_t cc, uint8_t value){
  if(cccodes[cc] && ccvalues[cc] != value){
    writer.controlChange(cccodes[cc], value);
    ccvalues[cc] = value;
  }
}

void updateSettings(){
  noteOff();
  writer.allNotesOff();
}

// void updateEvent(){
//   event.trigger = breath > MPX_THRESH;
//   event.modulation = getAnalogValue(FSR_PIN) >> 3;
//   event.volume = getAnalogValue(AMP_PIN) >> 3;
//   event.note = getStripPitch();
// }

boolean amptrig = true;

void loop(){
  if(button.check()){
    if(button.state == UP)
      amptrig = true;
    else if(button.state == DOWN)
      amptrig = false;
  }
  pot = getAnalogValue(POT_PIN);
  if(button.state == MIDDLE){
    if(pot > POT_THRESH)
      event.pitchbend = CENTER_PITCHBEND+(pot<<3); // range: none to full positive bend
    // leave event.note at previous value
  }else{
    event.pitchbend = 0;
    if(pot > POT_THRESH)
      event.note = getStripPitch();
  }
  if(amptrig){
    event.trigger = getAnalogValue(MPX_PIN) > MPX_THRESH;
    event.volume = (getAnalogValue(AMP_PIN) >> 3);
    event.modulation = (getAnalogValue(FSR_PIN) >> 3);
  }else{
    event.trigger = getAnalogValue(FSR_PIN) > FSR_THRESH;
    event.volume = (getAnalogValue(FSR_PIN) >> 3);
    event.modulation = (getAnalogValue(AMP_PIN) >> 3);
  }
  analogWrite(LAMP_PIN, event.volume<<1);
  if(event.trigger){
    if(state == STANDBY_STATE){
      noteOn();
      state = SUSTAIN_STATE;
    }else if(note != event.note){
      noteOn();
    }else if(button.state != MIDDLE){
      event.pitchbend = CENTER_PITCHBEND+((pot-lastpot)<<3);
    }
    if(event.pitchbend != pitchbend){
      writer.pitchBend(event.pitchbend);
      pitchbend = event.pitchbend;
    }
    updateController(CNTRL_VOLUME, event.volume);
  }else if(state != STANDBY_STATE){
    noteOff();
    state = STANDBY;
  }
  updateController(CNTRL_MODULATION, event.modulation);
  // brutal debounce / message output rate limiter.
  while(millis() - previousMillis < SERIAL_WRITE_INTERVAL);
  previousMillis = millis();
  event.pitchbend = CENTER_PITCHBEND;
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
