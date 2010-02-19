#include "spi.h"

#include <avr/io.h>

// this didn't work because pins weren't initialised properly

#define CS_DDR   DDRD
#define CS_PIN   PD2       // CS slave select pin
#define CS_PORT  PORTD

// #define CS_DDR   DDRB
// #define CS_PIN   PB1       // CS slave select pin
// #define CS_PORT  PORTB

#define SCK_DDR  DDRB
#define SCK_PIN  PB5      // SCK clock pin
#define SCK_PORT PORTB
#define SDI_DDR  DDRB
#define SDI_PIN  PB3      // SDI MOSI pin: master out, slave in
#define SDI_PORT PORTB

static void spi_transfer(uint8_t value) {
  for(uint8_t i=0x80; i; i>>=1){
    if(value & i)
      SDI_PORT |= _BV(SDI_PIN);
    else
      SDI_PORT &= ~_BV(SDI_PIN);
    // clock
    SCK_PORT |= _BV(SCK_PIN); // HIGH
    SCK_PORT &= ~_BV(SCK_PIN); // LOW
  }
}

void spi_out(uint8_t high, uint8_t low) {
  CS_PORT &= ~_BV(CS_PIN); // LOW
  spi_transfer(high);
  spi_transfer(low);
  CS_PORT |= _BV(CS_PIN); // HIGH
}

void spi_init(){
  // set pins to output
  CS_DDR |= _BV(CS_PIN);
  SCK_DDR |= _BV(SCK_PIN);
  SDI_DDR |= _BV(SDI_PIN);
  // hold slave select 1 pin high, so that chip is not selected to begin with
  CS_PORT |= _BV(CS_PIN); // HIGH
}
