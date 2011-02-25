#include "MidiCvInterface.h"

#include <wiring.h>
#include <avr/interrupt.h>

#include "device.h"

// #define LED1_PIN     2
// #define LED2_PIN     3

#define CV4_PIN      5 // cc 1
#define CV5_PIN      6 // cc 2
#define GATE1_PIN    8 // note on/off
#define GATE2_PIN    7 // sync: midi clock

// uint8_t note_range = 60;
uint8_t ref_note;
uint8_t tick;

uint8_t cv4, cv5;

void timer_setup(){
  //Timer2 setup  This is the audio rate timer, fires an interrupt at 15625 Hz sampling rate
  TIMSK2 = 1<<OCIE2A;  // interrupt enable audio timer
  OCR2A = 127;
  TCCR2A = 2;               // CTC mode, counts up to 127 then resets
  TCCR2B = 0<<CS22 | 1<<CS21 | 0<<CS20;   // different for atmega8 (no 'B' i think)
}

void MidiCvInterface::init(){
  pinMode(CV4_PIN, OUTPUT);
  pinMode(CV5_PIN, OUTPUT);
//   pinMode(LED1_PIN, OUTPUT);
//   pinMode(LED2_PIN, OUTPUT);
  pinMode(GATE1_PIN, OUTPUT);
  pinMode(GATE2_PIN, OUTPUT);
  ref_note = 36;
  factor = 0x66;
  clock_divider = 24;
  staccato = true;
  dac1.init(DAC1_CS_PIN, DAC_SHDN_BIT);
  dac2.init(DAC2_CS_PIN, DAC_SHDN_BIT);
  dac3.init(DAC3_CS_PIN, DAC_SHDN_BIT | DAC_A_B_BIT);
  timer_setup();
}

uint16_t MidiCvInterface::getCV(uint8_t cv){
  if(cv == 1)
    return cv1;
  else if(cv == 2)
    return cv2;
  else if(cv == 3)
    return cv3;
  else if(cv == 4)
    return cv4;
  else if(cv == 5)
    return cv4;
}

void MidiCvInterface::setCV(uint8_t cv, uint16_t value){
  switch(cv){
  case 1:
    cv1 = value;
    dac1.send(cv); 
    break;
  case 2:
    cv2 = value;
    dac2.send(cv);
    break;
  case 3:
    cv3 = value;
    dac3.send(cv);
    break;
  case 4:
    cv4 = value;
    break;
  case 5:
    cv5 == value;
    break;
  }
}

uint16_t MidiCvInterface::note_to_cv(uint8_t note){
  return (note-ref_note)*factor;
}

// scale min-max to min2-max2: 
// value = min2 + (value - min) * (max2 - min2) / (max - min)

int16_t MidiCvInterface::pitchbend_to_cv(uint16_t pb){
  //  scale 0, 16383 to -2, 2 semitones
  //   return value*4*factor/16384 - 2*factor;
  return pb*factor/4096 - 2*factor;
  // with a factor of 0x59, an unsigned 16 bit value overflows at 736
}  

void MidiCvInterface::channelPressure(int value){
//   if(value)
//     digitalWrite(LED1_PIN, HIGH);
//   else
//     digitalWrite(LED1_PIN, LOW);
}

void MidiCvInterface::startSong(){
  tick = 0;
  digitalWrite(GATE2_PIN, HIGH);
}

void MidiCvInterface::stopSong(){
  digitalWrite(GATE2_PIN, LOW);
}

void MidiCvInterface::midiClock(){
  if(++tick == clock_divider){
    digitalWrite(GATE2_PIN, HIGH);
    digitalWrite(GATE2_PIN, LOW);
    tick = 0;
  }
}

/*
  Standard MIDI Files use a pitch wheel range of +/-2 semitones = 200 cents. MIDI pitch bend wheel resolution (according to the spec) is +8192/-8191. That means there are 8192/200 = 40.96 pitch bend units to 1 cent
*/
/*
  The two bytes of the pitch bend message form a 14 bit number, 0 to 16383. The value 8192 (sent, LSB first, as 0x00 0x40), is centered, or "no pitch bend."
*/
void MidiCvInterface::pitchBend(int16_t value){
  cv1 += pitchbend_to_cv(value);
  dac1.send(cv1); 
}

void MidiCvInterface::controlChange(int cc, int value){
  switch(cc){
  case 1:
    setCV(2, value << 5);
    break;
  case 2:
    setCV(3, value << 5);
    break;
  case 3:
    cv4 = value<<1;
    break;
  case 4:
    cv5 = value<<1;
    break;
  case 100:
    ref_note = value;
    break;
  case 101:
    factor = value;
    break;
  case 102:
    clock_divider = value;
    break;
  }
}

void MidiCvInterface::noteOff(int note, int velocity){
//   if(note < ref_note || note > ref_note+note_range)
//     return;
  if(keydowns && --keydowns) // keydowns underrun protection
    return; // there are still some keys pressed
//   cv4 = velocity<<1;
  digitalWrite(GATE1_PIN, LOW);
//   digitalWrite(LED2_PIN, LOW);
}

void MidiCvInterface::noteOn(int note, int velocity){
//   if(note < ref_note || note >= ref_note+note_range)
//     return;

  if(velocity == 0)
    return noteOff(note, velocity);

  //  1v/octave with 0-5v means 5 octave, 60 semitone range.
  // scale ref_note - ref_note+60 to 0-4096 (12 bits)
  cv1 = note_to_cv(note);
  dac1.send(cv1);

//   cv4 = velocity<<1;

  if(keydowns++ && staccato)
    digitalWrite(GATE1_PIN, LOW);
  digitalWrite(GATE1_PIN, HIGH);
//   digitalWrite(LED2_PIN, HIGH);
}

void MidiCvInterface::allNotesOff(){
  keydowns = 0;
  cv1 = 0;
  cv2 = 0;
  cv3 = 0;
  dac1.send(cv1);
  dac2.send(cv2);
  dac3.send(cv3);
  cv4 = 0;
  cv5 = 0;
  digitalWrite(GATE1_PIN, LOW);
//   digitalWrite(LED2_PIN, LOW);
  digitalWrite(GATE2_PIN, LOW);
}

// timer 2 is audio interrupt timer
ISR(TIMER2_COMPA_vect) {
  OCR2A = 127;
  // todo: fix interference from millis() timer.
  if(cv4)
    analogWrite(CV4_PIN, cv4);
  else
    digitalWrite(CV4_PIN, LOW);
  if(cv5)
    analogWrite(CV5_PIN, cv5);
  else
    digitalWrite(CV5_PIN, LOW);
}
