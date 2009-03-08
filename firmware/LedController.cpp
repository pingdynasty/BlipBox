#include "LedController.h"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <wiring.h>

#define TLC_DDR        DDRB
#define TLC_PORT       PORTB
#define SCLK_PIN       PB0
#define XLAT_PIN       PB1
#define BLANK_PIN      PB2
#define GSCLK_PIN      PB3
#define VPRG_PIN       PB4 // also counter reset
#define SIN_PIN        PB5

uint8_t rowOffsets[] = {
  2, 1, 0, 3, 4, 
  3, 1, 0, 2, 4 
};

uint8_t colOffsets[] = {
  6, 4, 7, 5, 1, 3, 0, 2, 
  10, 8, 11, 9, 13, 15, 12, 14
};

/* send 6 bits from an 8 bit value over the TLC5940 data line */
static void shift6bits(uint8_t value) {
  for (uint8_t i=0x20;i;i>>=1){
    if(value & i)
      TLC_PORT |= _BV(SIN_PIN);
    else
      TLC_PORT &= ~_BV(SIN_PIN);
    TLC_PORT |= _BV(SCLK_PIN);
    TLC_PORT &= ~_BV(SCLK_PIN);
  }
}

/* send 10 bits over the TLC5940 data line using an 8 bit value 
   and 4 (least significant) bits padding */
static void shift12bits(uint8_t value) {
  for(uint8_t i=0x80; i; i>>=1){
    if(value & i)
      TLC_PORT |= _BV(SIN_PIN);
    else
      TLC_PORT &= ~_BV(SIN_PIN);
    // clock
    TLC_PORT |= _BV(SCLK_PIN);
    TLC_PORT &= ~_BV(SCLK_PIN);
  }
  // shift out four more zeros or ones for the 4 least significant bits
  if(value)
    TLC_PORT |= _BV(SIN_PIN);
  else
    TLC_PORT &= ~_BV(SIN_PIN);
  for(uint8_t i=0; i<4; ++i){
    TLC_PORT |= _BV(SCLK_PIN);
    TLC_PORT &= ~_BV(SCLK_PIN);
  }
}

void LedController::setDiagonalCross(uint8_t row, uint8_t col, uint8_t value){
  //   row goes to 10, ie row is x coordinate on screen
//   uint8_t d1 = row - col;
//   uint8_t d2 = 7 - row - col;
//   for(uint8_t i=0; i<8; ++i){
//     if(i+d1 >= 0 && i+d1 < 10)
// //       setLed(i+d1, i, value);
//       setLed(i+d1, i, value * abs(i-col) / 7);
//     if(i-d2 >= 0 && i-d2 < 10)
// //       setLed(i-d2, 7-i, value);
//       setLed(i-d2, 7-i, value * abs(7-i-col) / 7);
//   }
  for(int8_t i=-8; i<8; ++i){
    setLed(row+i, col+i, value / (2*abs(i) + 1));
    setLed(row-i, col+i, value / (2*abs(i) + 1));
  }
}

// todo: set blob with 10 or 8 bit precision location
void LedController::setBlob(uint8_t row, uint8_t col, uint8_t value){
  for(uint8_t i=-2; i<2; ++i){
    setLed(row+i, col+i, value / (2*abs(i) + 1));
    setLed(row-i, col+i, value / (2*abs(i) + 1));
  }
}

void LedController::setCross(uint8_t row, uint8_t col, uint8_t value){
  for(uint8_t i=0; i<10; ++i)
    setLed(i, col, value / (4*abs(row-i) + 1));
  for(uint8_t i=0; i<8; ++i)
    setLed(row, i, value / (4*abs(col-i) + 1));
}

void LedController::setStar(uint8_t row, uint8_t col, uint8_t value){
  setLed(row+1, col, value);
  setLed(row, col+1, value);
  setLed(row-1, col, value);
  setLed(row, col-1, value);
}

void LedController::setSquare(uint8_t row, uint8_t col, uint8_t value){
  setStar(row, col, value);
  value >>= 1;
  setLed(row+1, col+1, value);
  setLed(row-1, col+1, value);
  setLed(row+1, col-1, value);
  setLed(row-1, col-1, value);
}

void LedController::setLed(uint8_t row, uint8_t col, uint8_t value){
  if(col < 8){
    if(row < 5)
      leds[rowOffsets[row]].setLed(colOffsets[col], value);
    else if(row < 10)
      leds[rowOffsets[row]].setLed(colOffsets[col+8], value);
  }
}

/* set the dot correction value for all pins to a value between 0 and 63 */
void LedController::setGlobalDotCorrection(uint8_t value) {
  TLC_PORT |= _BV(VPRG_PIN);
  for(uint8_t i=0;i<16;i++)
    shift6bits(value);
  TLC_PORT |= _BV(XLAT_PIN);
  TLC_PORT &= ~_BV(XLAT_PIN);
  TLC_PORT &= ~_BV(VPRG_PIN);

  doExtraPulse = true;
  counter.reset();
}

void LedController::init() {
  counter.init();
//   counter.reset(); // reset called in setGlobalDotCorrection()

  cli();
  // set pins to output
  TLC_DDR |= 
    _BV(BLANK_PIN) | _BV(XLAT_PIN) | _BV(SCLK_PIN) | _BV(GSCLK_PIN) |
    _BV(SIN_PIN);

  TLC_PORT &= ~_BV(BLANK_PIN);   // blank everything until ready
  TLC_PORT &= ~_BV(XLAT_PIN);
  TLC_PORT &= ~_BV(SCLK_PIN);
  TLC_PORT &= ~_BV(GSCLK_PIN);

  setGlobalDotCorrection(63); // Max intensity.

  // PWM timer
  TCCR2A = (_BV(WGM21) |   // CTC 
            _BV(COM2A0));  // toggle OC2A on match -> GSCLK
  TCCR2B = _BV(CS20);      // No prescaler
  OCR2A = 1;               // toggle every timer clock cycle -> 4 MHz
  TCNT2 = 0;

  // Latch timer
  TCCR1A = (_BV(WGM10));   // Fast PWM 8-bit
  
  // div 64 prescaler
  // Fast PWM 8-bit  : 1/4096th of OC2A
  TCCR1B = (_BV(CS11) | _BV(CS10) | _BV(WGM12));

  // Enable overflow interrupt
  TIMSK1 = _BV(TOIE1);     
  TCNT1 = 0;

  sei();
}

void LedController::brighten(uint8_t factor){
  for(uint8_t row=0; row<ROWS; ++row)
    for(uint8_t i=0;i<FRAME_LENGTH;i++)
      leds[row].values[i] <<= factor;
}

void LedController::fade(uint8_t factor){
  for(uint8_t row=0; row<ROWS; ++row)
    for(uint8_t i=0;i<FRAME_LENGTH;i++)
      leds[row].values[i] >>= factor;
//       leds[row].values[i] = leds[row].values[i] > factor ? leds[row].values[i] - factor : 0;
}

void LedController::clear(){
  for(uint8_t row=0; row<ROWS; ++row)
    leds[row].clear();
}

void LedRow::clear(){
  for(uint8_t i=0;i<FRAME_LENGTH;i++)
    values[i] = 0;
}

void LedController::displayCurrentRow() {
  const LedRow& row = leds[counter.getCurrentRow()];
  // shift data out

  for(uint8_t i=0;i<FRAME_LENGTH;i++)
    shift12bits(row.values[i]);

  // blank
  TLC_PORT |= _BV(BLANK_PIN);

  counter.increment();

  // latch
  TLC_PORT |= _BV(XLAT_PIN);
  TLC_PORT &= ~_BV(XLAT_PIN);
    
  // Extra SCLK pulse according to Datasheet p.18
  if(doExtraPulse){
    TLC_PORT |= _BV(SCLK_PIN);
    TLC_PORT &= ~_BV(SCLK_PIN);
    doExtraPulse = false;
  }

  // unblank
  TLC_PORT &= ~_BV(BLANK_PIN);
}