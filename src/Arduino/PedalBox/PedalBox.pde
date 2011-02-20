
#include "adc_freerunner.h"
#ifndef ADC_CHANNELS
#define  ADC_CHANNELS 6
#endif

#include "MidiReader.h"
#include "MidiWriter.h"

#define  EXPR1A_PIN  5 // PC5
#define  EXPR1B_PIN  4 // PC4
#define  EXPR2A_PIN  3 // PC3
#define  EXPR2B_PIN  2 // PC2

//#define  SWITCH1_PIN 7 // PD7
//#define  SWITCH2_PIN 8 // PD8

#define ADC_MIN 0
#define ADC_MAX 1023

class CommandInterface : public MidiInterface {
public:
};

CommandInterface command;
MidiReader midiin;
MidiWriter midiout;

#define SWITCH_TOGGLE_MODE 1
#define SWITCH_ON_OFF_MODE 2
#define SWITCH_OFF_ON_MODE 3

class SwitchValue {
private:
  bool value;
  uint8_t note;
  uint8_t mode;

public:
  void init(uint8_t nnote){
    value = false;
    note = nnote;
    mode = SWITCH_TOGGLE_MODE;
  }
  
  void update(bool nval){
    switch(mode){
    case SWITCH_TOGGLE_MODE:
      nval = !value;
      break;
    case SWITCH_ON_OFF_MODE:
      // do naught
      break;
    case SWITCH_OFF_ON_MODE:
      nval = !nval;
      break;
    }
    if(value != nval){
      value = nval;
      if(value)
        midiout.noteOn(note, 127);
      else
        midiout.noteOff(note, 0); 
    }
  }     
};

class ContinuousValue {
private:
  uint16_t value;
  uint16_t minv;
  uint16_t maxv;
  uint8_t cc;

  uint16_t scale(uint16_t nval){
    return minv + (nval - ADC_MIN) * (maxv - minv) / (ADC_MAX - ADC_MIN);
  }

public:
  void init(uint8_t ncc){
    value = 0;
    minv = 0;
    maxv = 127;
    cc = ncc;
  }

  void update(uint16_t nval){
    nval = scale(nval);
    if(value != nval){
      value = nval;
      midiout.controlChange(cc, value);
    }
  }
};

#define CONTINUOUS_COUNT 2
#define SWITCH_COUNT 2

ContinuousValue cvals[ADC_CHANNELS];
SwitchValue svals[SWITCH_COUNT];

#define SWITCH1_PIN 2
#define SWITCH2_PIN 3

void setup() {
  cli(); // disable interrupts

  setup_adc();
  midiin.init(&command);
  midiout.init(1);

  for(int i=0; i<ADC_CHANNELS; ++i)
    cvals[i].init(i+60);

  for(int i=0; i<SWITCH_COUNT; ++i)
    svals[i].init(40+i);

  /*
  pinMode(SWITCH1_PIN, INPUT);           // set pin to input
  digitalWrite(SWITCH1_PIN, HIGH);       // turn on pullup resistor
  pinMode(SWITCH2_PIN, INPUT);           // set pin to input
  digitalWrite(SWITCH2_PIN, HIGH);       // turn on pullup resistor
  attachInterrupt(0, blink, CHANGE);
  */
  beginSerial(38400);
//     beginSerial(31250);
  midiout.noteOn(1, 0);
  midiout.noteOff(1, 0);

  sei(); // enable interrupts
} 
  
void loop() {
  for(int i=2; i<ADC_CHANNELS; ++i){
//     cvals[i].update(analogRead(i));
    cvals[i].update(getAnalogValue(i));
  }

  svals[0].update(digitalRead(SWITCH1_PIN));
  svals[1].update(digitalRead(SWITCH2_PIN));

  delay(300);
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
  midiin.read(c);
}
