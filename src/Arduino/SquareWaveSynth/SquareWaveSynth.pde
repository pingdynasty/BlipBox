/*
  Output on pins 3 (pulse width modulated)
  and 11 (1/2 frequency at 50% duty cycle).
  Wire output pins and gnd to a pot for volume control.
 
  Frequency calculation
  pin 11 = 16MHz / prescaler / limit / 4
  pin 3  =  16MHz / prescaler / limit / 2

  see http://blipbox.org/blog/2011/05/23/arduino-squarewave-generation/
*/

#include "MidiInterface.h"
#include "MidiReader.h"
#include "SquareWave.h"

#define DUTY_PIN     3
#define FREQ_PIN     4
#define LED_PIN      13
#define SERIAL_SPEED 57600

SquareWave swave;

class CommandInterface : public MidiInterface {
private:
  int8_t keydowns;
  int8_t note;
public:
  void noteOn(int note, int velocity){
    if(velocity == 0)
      return noteOff(note, velocity);
    ++keydowns;
    swave.setMidiNote(note);
    digitalWrite(LED_PIN, HIGH);
  }

  void controlChange(int cc, int value){
    if(cc == 1)
      swave.setDutyCycle(0.5-value/254.0); // map 0-127 to 0.5-0.0
  }

  void allNotesOff(){
    keydowns = 0;
    soundOff();
    swave.setDutyCycle(0.5);
  }

  void noteOff(int note, int velocity){
    if(keydowns && --keydowns)
      return; // there are still some keys pressed
    soundOff();
  }

  void soundOff(){
    swave.setPrescaler(0);
    digitalWrite(LED_PIN, LOW);
  }
};

CommandInterface command;
MidiReader reader;

void setup(){
  reader.init(&command);
  beginSerial(SERIAL_SPEED);
  pinMode(LED_PIN, OUTPUT);
  swave.init();
}

void loop(){
  // do nothing.
}

/* Serial RX interrupt */
#if defined(__AVR_ATmega168__)
SIGNAL(SIG_USART_RECV){
  unsigned char c = UDR0;
#else
SIGNAL(SIG_UART_RECV){
  unsigned char c = UDR;
#endif
  reader.read(c);
}
