/** 
    Expression pedal / footswitch to MIDI interface.

    vero board:
    PC2 / PC5 stereo 1 (far side)
    PC3 / PC4 stereo 2
    PD5 mono 1
    PD6 mono 2 (far side)
    ------------------------
    send MIDI note off to turn output off, input on
    note on with velocity 0 to turn input/output off
    note on with velocity>0 to turn output on (with PWM for footswitches).
    CC 62 analog2 PC2
    CC 65 analog5 PC5
    CC 63 analog3 PC3
    CC 64 analog4 PC4
    Note 40 SWITCH0_PIN digital 5 PD5
    Note 41 SWITCH1_PIN digital 6 PD6

stereo jack 0 tip CC65 / PC5 ring CC62 / PC2
stereo jack 1 tip CC63 / PC3 ring CC64 / PC4

todo: CC 50-55 sets analog0-5 min value, CC 70-75 sets analog0-5 max value.
*/

#include "adc_freerunner.h"
#ifndef ADC_CHANNELS
#define  ADC_CHANNELS 6
#endif

/* interval at which to write sensor output (milliseconds) */
#define SERIAL_WRITE_INTERVAL 20L
// #define SERIAL_SPEED 31250
#define SERIAL_SPEED 38400
unsigned long previousMillis = 0;        // will store last time write was done

#include "MidiReader.h"
#include "MidiWriter.h"

// #define  EXPR1A_PIN  5 // PC5
// #define  EXPR1B_PIN  4 // PC4
// #define  EXPR2A_PIN  3 // PC3
// #define  EXPR2B_PIN  2 // PC2

#define ADC_MIN 0
#define ADC_MAX 1023

MidiReader midiin;
MidiWriter midiout;

#define SWITCH_TOGGLE_MODE 1
#define SWITCH_ON_OFF_MODE 2
#define SWITCH_OFF_ON_MODE 3

class MessageSender {
public:
  virtual bool send();
};

enum ControlMode { INPUT_MODE, OFF_MODE, ON_MODE };

class SwitchValue : public MessageSender {
private:
  uint8_t pin;
  uint8_t value;
  uint8_t previous;
  uint8_t note;
  ControlMode mode;

public:
  void init(uint8_t npin, uint8_t nnote){
    pin = npin;
    note = nnote;
    value = false;
    setMode(INPUT_MODE);
  }
  void setMode(ControlMode nmode){
    mode = nmode;
    switch(mode){      
    case INPUT_MODE:
      pinMode(pin, INPUT);           // set pin to input
      digitalWrite(pin, HIGH);       // turn on pullup resistor
      break;
    case ON_MODE:
      pinMode(pin, OUTPUT);
      digitalWrite(pin, HIGH);
      break;
    case OFF_MODE:
      digitalWrite(pin, LOW);
      break;
    }
  }
  void on(uint8_t velocity){
    if(velocity){
      setMode(ON_MODE);
      analogWrite(pin, velocity<<1);
    }else
      setMode(OFF_MODE);
  }  
  void update(uint8_t nval){
    value = nval;
  }
  bool send(){
    if(mode == INPUT_MODE){
      if(value != previous){
        previous = value;
        if(value == LOW)
          midiout.noteOn(note, 80);
        else
          midiout.noteOff(note, 0); 
        return true;
      }
    }
    return false;
  }
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
//   }     
};

class ContinuousValue : public MessageSender {
private:
  uint8_t pin;
  uint16_t value;
  uint8_t previous;
//   uint8_t samples;
  uint8_t cc;
  ControlMode mode;

public:
  void init(uint8_t npin, uint8_t ncc){
    pin = npin;
    value = 0;
    cc = ncc;
    previous = 128;
    setMode(INPUT_MODE);
  }
  void setMode(ControlMode nmode){
    mode = nmode;
    switch(mode){
    case INPUT_MODE:
      DDRC &= ~_BV(pin); // input mode
      PORTC |= _BV(pin); // hi-z
//       PORTC &= ~_BV(pin); // low-z
      break;
    case ON_MODE:
      DDRC |= _BV(pin);
      PORTC |= _BV(pin);
      break;
    case OFF_MODE:
      PORTC &= ~_BV(pin);
      break;
    }
  }
  void on(uint8_t velocity){
    if(velocity)
      setMode(ON_MODE);
    else
      setMode(OFF_MODE);
  }  
  void update(uint16_t nval){
//     if(samples < 9){
//       value += nval;
//       ++samples;
//     }else{
//       value = (value>>1)+nval;
//     }
//     value = nval*127/1023;
    value = nval>>3;
  }
  bool send(){
    if(mode == INPUT_MODE && value != previous){
      previous = value;
      midiout.controlChange(cc, value);
      return true;
    }
    return false;
  }
//     if(mode == INPUT_MODE){
//       value >>= samples;
//       samples = 3;
//       if(value != previous){
//         previous = value;
//         midiout.controlChange(cc, value);
//         return true;
//       }
//     }
//     return false;
//   }
};

// #define CONTINUOUS_COUNT 2
#define SWITCH_COUNT 2

ContinuousValue cvals[ADC_CHANNELS];
SwitchValue svals[SWITCH_COUNT];

#define SWITCH0_PIN 5 // PD5
#define SWITCH1_PIN 6 // PD6

class CommandInterface : public MidiInterface {
public:
  void noteOn(int note, int velocity){
    if(note-40>=0 && note-40<SWITCH_COUNT)
      svals[note-40].on(velocity);
    else if(note-60>=0 && note-60<ADC_CHANNELS)
      cvals[note-60].on(velocity);
  }
  void noteOff(int note, int velocity){
    if(note-40>0 && note-40<SWITCH_COUNT)
      svals[note-40].setMode(INPUT_MODE);
    else if(note-60>0 && note-60<ADC_CHANNELS)
      cvals[note-60].setMode(INPUT_MODE);
  }
};

CommandInterface command;

#define MESSAGE_SENDER_COUNT 6
class MessageDispatcher {
private:
  MessageSender* msgs[MESSAGE_SENDER_COUNT];
  uint8_t index;
public:
  MessageDispatcher(){
    msgs[0] = &svals[0];
    msgs[1] = &svals[1];
    msgs[2] = &cvals[2];
    msgs[3] = &cvals[3];
    msgs[4] = &cvals[4];
    msgs[5] = &cvals[5];
  }
  bool send(){
    uint8_t cnt = MESSAGE_SENDER_COUNT;
    while(cnt--){
      if(++index == MESSAGE_SENDER_COUNT)
        index = 0;
      if(msgs[index]->send())
        return true;
    }
    return false;
  }
};

MessageDispatcher dispatcher;

void setup() {
  cli(); // disable interrupts

#ifdef _ANALOGREADER_H_
  setup_adc();
#endif

  midiin.init(&command);
  midiout.init(1);

  cvals[2].init(PC2, 62);
  cvals[3].init(PC3, 63);
  cvals[4].init(PC4, 64);
  cvals[5].init(PC5, 65);

  cvals[3].setMode(ON_MODE); // turn PC3 on
  cvals[5].setMode(ON_MODE); // turn PC5 on

  svals[0].init(SWITCH0_PIN, 40);
  svals[1].init(SWITCH1_PIN, 41);

//   attachInterrupt(0, blink, CHANGE);

  beginSerial(SERIAL_SPEED);
  // say hello
  midiout.noteOn(1, 0);
  midiout.noteOff(1, 0);

  sei(); // enable interrupts
} 
  
void loop() {
  for(uint8_t i=2; i<ADC_CHANNELS; ++i)
    cvals[i].update(getAnalogValue(i));

  svals[0].update(digitalRead(SWITCH0_PIN));
  svals[1].update(digitalRead(SWITCH1_PIN));

  if(millis() - previousMillis > SERIAL_WRITE_INTERVAL){
    if(dispatcher.send())
      previousMillis = millis();
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
  midiin.read(c);
}
