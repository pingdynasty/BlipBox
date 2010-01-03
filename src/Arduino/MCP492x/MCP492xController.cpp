#include "MCP492xController.h"
// #include <avr/io.h>
#include <wiring.h>

#define DAC_A_B_BIT  7
#define DAC_BUF_BIT  6
#define DAC_GA_BIT   5
#define DAC_SHDN_BIT 4

#define CS_PIN   9       // CS slave select pin
#define SCK_PIN  13      // SCK clock pin
#define SDI_PIN  11      // SDI MOSI pin: master out, slave in

static void spi_transfer(uint8_t working) {
  // function to actually bit shift the data byte out
  for(int i = 1; i <= 8; i++) {
    if(working > 127)
      digitalWrite (SDI_PIN, HIGH); // if it is a 1 (ie. B1XXXXXXX), set the master out pin high
    else
      digitalWrite (SDI_PIN, LOW); // if it is not 1 (ie. B0XXXXXXX), set the master out pin low
    digitalWrite (SCK_PIN, HIGH); // set clock high, the pot IC will read the bit into its register
    working = working << 1;
    digitalWrite(SCK_PIN, LOW); // set clock low, the pot IC will stop reading and prepare for the next iteration (next significant bit
  }
}

static void spi_out(uint8_t high, uint8_t low) {
  digitalWrite(CS_PIN, LOW);
  spi_transfer(high);
  spi_transfer(low);
  digitalWrite(CS_PIN, HIGH);
}

static void spi_init() {
  pinMode(CS_PIN, OUTPUT); // set CS pin to output
  pinMode(SCK_PIN, OUTPUT); // set SCK pin to output
  pinMode(SDI_PIN, OUTPUT); // set MOSI pin to output
  digitalWrite(CS_PIN, HIGH); // hold slave select 1 pin high, so that chip is not selected to begin with
}

/*
// this didn't work because pins weren't initialised properly

#define CS_DDR   DDRB
#define CS_PIN   PB1       // CS slave select pin
#define CS_PORT  PORTB
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

static void spi_out(uint8_t high, uint8_t low) {
  CS_PORT &= ~_BV(CS_PIN); // LOW
  spi_transfer(high);
  spi_transfer(low);
  CS_PORT |= _BV(CS_PIN); // HIGH
}

static void spi_init(){
  // set pins to output
  CS_DDR |= _BV(CS_PIN);
  SCK_DDR |= _BV(SCK_PIN);
  SDI_DDR |= _BV(SDI_PIN);
  // hold slave select 1 pin high, so that chip is not selected to begin with
  CS_PORT |= _BV(CS_PIN); // HIGH
}
*/

#define TRANSFER_BITS _BV(DAC_SHDN_BIT)
//#define TRANSFER_BITS (_BV(DAC_SHDN_BIT) | _BV(DAC_GA_BIT))

void MCP492xController::init(){
  spi_init();
}

void MCP492xController::send(uint8_t data){
  spi_out(TRANSFER_BITS, data & 0xff);
}

void MCP492xController::send(uint8_t high, uint8_t low){
  spi_out(TRANSFER_BITS | (high & 0xf), low & 0xff);
}

void MCP492xController::send(uint16_t data){
  spi_out(TRANSFER_BITS | (data >> 8 & 0xf), data & 0xff);
}
