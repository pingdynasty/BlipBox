#include "MidiInterface.h"
#include "MidiReader.h"
#include "MidiWriter.h"
#include "adc_freerunner.h"

MidiReader reader;
MidiWriter writer;

#define SENSOR_MAX          1023

#define AMP_PIN 5
#define MPX_PIN 4
#define POT_PIN 3
#define FSR_PIN 2

#define MPX_THRESH 40
// MPX5010DP pressure sensor, 5v single supply.

#define SERIAL_WRITE_INTERVAL 20L

unsigned long previousMillis = 0;        // will store last time write was done

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

uint16_t pot;

#define OP1     1
#define OP2     2
#define OP3     3
uint8_t operation;

#define CC_MODULATION    0
#define CC_VOLUME        1

uint8_t CC_CODES_SET[] = { 1, 2 };

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
  event.pitchbend = 8192;
  cccodes = CC_CODES_SET;

  cli(); // disable interrupts

  setup_adc();

  writer.init(1); // write output to MIDI channel 1
  reader.init(&command);

  sei(); // enable interrupts

//   Serial.begin(9600);
  beginSerial(38400);
//     beginSerial(31250);

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
  noteOff();
  writer.allNotesOff();
}

// void updateEvent(){
//   event.trigger = breath > MPX_THRESH;
//   event.modulation = getAnalogValue(FSR_PIN) >> 3;
//   event.volume = getAnalogValue(AMP_PIN) >> 3;
//   event.note = getStripPitch();
// }

void loop(){

  pot = getAnalogValue(POT_PIN);
  event.trigger = getAnalogValue(MPX_PIN) > MPX_THRESH;
  event.volume = (getAnalogValue(AMP_PIN) >> 3);
  event.note = getStripPitch();
  event.modulation = (getAnalogValue(FSR_PIN) >> 3);

  if(event.trigger){
    if(state == STANDBY_STATE){
      noteOn();
      state = SUSTAIN_STATE;
    }else if(note != event.note){
      noteOn();
    }
    updateController(CC_VOLUME, event.volume);
//     if(event.pitchbend != pitchbend){
//       writer.pitchBend(event.pitchbend);
//       pitchbend = event.pitchbend;
//     }
  }else if(state != STANDBY_STATE){
    noteOff();
    state = STANDBY;
  }
  updateController(CC_MODULATION, event.modulation);

  // brutal debounce / message output rate limiter.
  while(millis() - previousMillis < SERIAL_WRITE_INTERVAL);
  previousMillis = millis();
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
