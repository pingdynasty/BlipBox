/** 
    Expression pedal / footswitch to MIDI interface.

    vero board:
    PC2 / PC5 stereo 1 (far side)
    PC3 / PC4 stereo 2
    PD5 mono 1
    PD6 mono 2 (far side)
*/

#include "adc_freerunner.h"
#ifndef ADC_CHANNELS
#define  ADC_CHANNELS 6
#endif

/* interval at which to write sensor output (milliseconds) */
#define SERIAL_WRITE_INTERVAL 20L
unsigned long previousMillis = 0;        // will store last time write was done

#include "MidiReader.h"
#include "MidiWriter.h"

// #define  EXPR1A_PIN  5 // PC5
// #define  EXPR1B_PIN  4 // PC4
// #define  EXPR2A_PIN  3 // PC3
// #define  EXPR2B_PIN  2 // PC2

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
  uint8_t value;
  uint8_t note;
//   uint8_t mode;

public:
  void init(uint8_t nnote){
    value = false;
    note = nnote;
//     mode = SWITCH_ON_OFF_MODE;
  }
  
  bool update(uint8_t nval){
    if(value != nval){
      value = nval;
      if(value == LOW)
        midiout.noteOn(note, 127);
      else
        midiout.noteOff(note, 0); 
      return true;
    }
    return false;
//     switch(mode){
//     case SWITCH_TOGGLE_MODE:
//       if(value != nval){
//         value = nval;
//       break;
//     case SWITCH_ON_OFF_MODE:
//       // do naught
//       break;
//     case SWITCH_OFF_ON_MODE:
//       nval = !nval;
//       break;
//     }
//     if(value != nval){
//       value = nval;
//       if(value)
//         midiout.noteOn(note, 127);
//       else
//         midiout.noteOff(note, 0); 
//     }
  }     
};

// class ContinuousValue {
// private:
//   uint16_t value;
//   int8_t previous;
//   uint8_t samples;
//   uint16_t minv;
//   uint16_t maxv;
//   uint8_t cc;
//   uint16_t scale(uint16_t nval){
//     return minv + (nval - ADC_MIN) * (maxv - minv) / (ADC_MAX - ADC_MIN);
//   }
// public:
//   void init(uint8_t ncc){
//     value = 0;
// //     minv = 0;
// //     maxv = 127;
//     cc = ncc;
//     previous = -1;
//   }
//   void update(uint16_t nval){
//     if(nval != value){
//       value = nval;
//       midiout.controlChange(cc, value);
//     }
//   }
// };

class ContinuousValue {
private:
  uint16_t value;
  int8_t previous;
  uint8_t samples;
  uint8_t cc;

public:
  void init(uint8_t ncc){
    value = 0;
    cc = ncc;
    previous = -1;
  }
  void update(uint16_t nval){
    if(samples < 9){
      value += nval;
      ++samples;
    }
  }
  bool send(){
    value >>= samples;
    samples = 3;
    if(value != previous){
      previous = value;
      midiout.controlChange(cc, value);
      return true;
    }
    return false;
  }
};

// class ExpressionPedal {
//   uint8_t tip, ring;
// };

// #define TIP_TO_RING_MODE 1
// #define RING_TO_TIP_MODE 2
// #define RING_TO_TIP_MODE 3

// class StereoJack {
// private:
//   uint8_t tip, ring;
//   uint8_t input;
// //   uint8_t mode;
// public:
//   void init(uint8_t ntip, uint8_t nring){
//     tip = ntip;
//     ring = nring;
//     input = ring;
//  }
//   void configure(uint8_t nmode){
//     mode = nmode;
//     switch(mode){
//     case TIP_TO_RING:
//       input = ring;
//       pinMode(, INPUT);
//       digitalWrite(SWITCH0_PIN, HIGH);
//       break;
//     }
//   }

//   uint16_t read(){
//     getAnalogValue(input);
//   }
// };

#define CONTINUOUS_COUNT 2
#define SWITCH_COUNT 2

ContinuousValue cvals[ADC_CHANNELS];
SwitchValue svals[SWITCH_COUNT];

#define SWITCH0_PIN 5 // PD5
#define SWITCH1_PIN 6 // PD6

void setup() {
  cli(); // disable interrupts

  setup_adc();
  midiin.init(&command);
  midiout.init(1);

//   DDRC |= _BV(PC2); // analog 2 out
//   PORTC |= (_BV(PC2)); // a2 high
//   DDRC |= _BV(PC4); // analog 4 out
//   PORTC |= (_BV(PC2) | _BV(PC3)); // set analog pins 3 and 5 high
//   pinMode(A3, OUTPUT);
//   digitalWrite(A3, HIGH);
//   pinMode(A5, OUTPUT);
//   digitalWrite(A5, HIGH);
  for(uint8_t i=0; i<ADC_CHANNELS; ++i)
    cvals[i].init(i+60);

//   for(uint8_t i=0; i<SWITCH_COUNT; ++i)
//     svals[i].init(40+i);

  svals[0].init(41);
  svals[1].init(42);
  pinMode(SWITCH0_PIN, INPUT);           // set pin to input
  digitalWrite(SWITCH0_PIN, HIGH);       // turn on pullup resistor
  pinMode(SWITCH1_PIN, INPUT);           // set pin to input
  digitalWrite(SWITCH1_PIN, HIGH);       // turn on pullup resistor

//   attachInterrupt(0, blink, CHANGE);

  beginSerial(38400);
//     beginSerial(31250);
  // say hello
  midiout.noteOn(1, 0);
  midiout.noteOff(1, 0);

  sei(); // enable interrupts
} 
  
void loop() {
//   for(uint8_t i=2; i<ADC_CHANNELS; ++i){
//     cvals[i].update(getAnalogValue(i));
//   }

  for(uint8_t i=2; i<ADC_CHANNELS; ++i)
    cvals[i].update(getAnalogValue(i));

//   cvals[2].update(getAnalogValue(2));
//   cvals[3].update(getAnalogValue(3));
//   cvals[4].update(getAnalogValue(4));
//   cvals[5].update(getAnalogValue(5));

//   svals[0].update(digitalRead(SWITCH0_PIN));
//   svals[1].update(digitalRead(SWITCH1_PIN));

  if(millis() - previousMillis > SERIAL_WRITE_INTERVAL){
    previousMillis = millis();   // remember the last time we did this
    if(svals[0].update(digitalRead(SWITCH0_PIN)))
      return;
    if(svals[1].update(digitalRead(SWITCH1_PIN)))
      return;
    for(uint8_t i=2; i<ADC_CHANNELS; ++i)
      if(cvals[i].send())
        return;
  }
//   delay(300);
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
