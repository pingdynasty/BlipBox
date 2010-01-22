#include "MidiReader.h"
#include "ConsoleReader.h"
#include "MidiCvInterface.h"

// PWM cv outputs square pulse wave (at the 15kHz sampling rate probably)
// vactrol? 100uF cap to ground seems to cut the noise.

MidiCvInterface cv;
// MidiReader reader;
ConsoleReader reader;

void setup(){
  cv.init();
  reader.init(&cv);
}

void loop() {
  reader.read();
}

/*
#define PIN_CS           9
#define PIN_SCK          13
#define PIN_MISO         12
#define PIN_MOSI         11

// the two bytes that go to the DAC over SPI
uint8_t dacSPI0;
uint8_t dacSPI1;

void spi_send(uint16_t value){
  value <<= 4;

  // format sample for SPI port
  dacSPI0 = value >> 8;
  dacSPI0 >>= 4;
  dacSPI0 |= 0x30;
  dacSPI1 = value >> 4;

  // transmit value out the SPI port
  PORTB &= ~(1<<1); // Frame sync low
  SPDR = dacSPI0;
  while (!(SPSR & (1<<SPIF)));
  SPDR = dacSPI1;
  while (!(SPSR & (1<<SPIF)));
  PORTB |= (1<<1); // Frame sync high
}

void spi_setup(){
  SPCR = 0x50;   // set up SPI port
  SPSR = 0x01;
  DDRB |= 0x2E;       // PB output for DAC CS, and SPI port
  PORTB |= (1<<1);
  sei();			// global interrupt enable
}

*/

/*
  pinMode(PIN_CS, OUTPUT);
 
 pinMode(PIN_SCK, OUTPUT);
 digitalWrite(PIN_SCK, LOW);
 pinMode(PIN_MOSI, OUTPUT);
 digitalWrite(PIN_MOSI, LOW);
 pinMode(PIN_MISO, INPUT);
 digitalWrite(PIN_MISO,HIGH);
 
 //  SPCR = B01110001;              // Auto SPI: no int, enable, LSB first, master, + edge, leading, f/16
 //  SPSR = B00000000;              // not double data rate
 
 // spi enabled, master mode, clock @ f/128 for init
 //SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1)|(1<<SPR0);
 
 //  enableSPI();
 
 digitalWrite(PIN_CS, LOW); // select chip
 }
 
 void enableSPI() {
 SPCR |= 1 << SPE;
 }
 
 void disableSPI() {
 SPCR &= ~(1 << SPE);
 }
 */

