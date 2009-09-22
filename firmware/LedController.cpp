#include "LedController.h"
#include "Characters.h"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <wiring.h>
#include "device.h"

#ifdef TLC_VPRG_PIN
/* send 6 bits from an 8 bit value over the TLC5940 data line */
static void shift6bits(uint8_t value) {
  for (uint8_t i=0x20;i;i>>=1){
    if(value & i)
      TLC_SIN_PORT |= _BV(TLC_SIN_PIN);
    else
      TLC_SIN_PORT &= ~_BV(TLC_SIN_PIN);
    TLC_SCLK_PORT |= _BV(TLC_SCLK_PIN);
    TLC_SCLK_PORT &= ~_BV(TLC_SCLK_PIN);
  }
}
#endif

/* send 10 bits over the TLC5940 data line using an 8 bit value 
   and 4 (least significant) bits padding */
static void shift12bits(uint8_t value) {
  for(uint8_t i=0x80; i; i>>=1){
    if(value & i)
      TLC_SIN_PORT |= _BV(TLC_SIN_PIN);
    else
      TLC_SIN_PORT &= ~_BV(TLC_SIN_PIN);
    // clock
    TLC_SCLK_PORT |= _BV(TLC_SCLK_PIN);
    TLC_SCLK_PORT &= ~_BV(TLC_SCLK_PIN);
  }
  // shift out four more zeros or ones for the 4 least significant bits
  if(value)
    TLC_SIN_PORT |= _BV(TLC_SIN_PIN);
  else
    TLC_SIN_PORT &= ~_BV(TLC_SIN_PIN);
  for(uint8_t i=0; i<4; ++i){
    TLC_SCLK_PORT |= _BV(TLC_SCLK_PIN);
    TLC_SCLK_PORT &= ~_BV(TLC_SCLK_PIN);
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
  for(int8_t i=-3; i<4; i+=2){
    setLed(row+i, col+i, value / (2*abs(i) + 1));
    setLed(row+i, col-i, value / (2*abs(i) + 1));
    setLed(row-i, col+i, value / (2*abs(i) + 1));
    setLed(row-i, col-i, value / (2*abs(i) + 1));
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

/* set the dot correction value for all pins to a value between 0 and 63 */
#ifdef TLC_VPRG_PIN
void LedController::setGlobalDotCorrection(uint8_t value) {
  TLC_VPRG_PORT |= _BV(TLC_VPRG_PIN);
  for(uint8_t i=0;i<16;i++)
    shift6bits(value);
  TLC_XLAT_PORT |= _BV(TLC_XLAT_PIN);
  TLC_XLAT_PORT &= ~_BV(TLC_XLAT_PIN);
  TLC_VPRG_PORT &= ~_BV(TLC_VPRG_PIN);
  doExtraPulse = true;
  counter.reset();
}
#endif

void LedController::init() {

  counter.init();
  counter.reset();

//   doExtraPulse = true;

  cli();
  // set pins to output
  TLC_XLAT_DDR |= _BV(TLC_XLAT_PIN);
  TLC_BLANK_DDR |= _BV(TLC_BLANK_PIN);
  TLC_GSCLK_DDR |= _BV(TLC_GSCLK_PIN);

  TLC_SIN_DDR |= _BV(TLC_SIN_PIN);
  TLC_SCLK_DDR |= _BV(TLC_SCLK_PIN);

 // set blank high
  TLC_BLANK_PORT |= _BV(TLC_BLANK_PIN);
  // put everything else low until ready
  TLC_XLAT_PORT &= ~_BV(TLC_XLAT_PIN);
  TLC_SCLK_PORT &= ~_BV(TLC_SCLK_PIN);
  TLC_GSCLK_PORT &= ~_BV(TLC_GSCLK_PIN);

#ifdef TLC_VPRG_PIN
  TLC_VPRG_DDR  |= _BV(TLC_VPRG_PIN);
  setGlobalDotCorrection(63); // Max intensity.
#endif

  // PWM timer
  TCCR2A = (_BV(WGM21) |   // CTC 
            _BV(TLC_GSCLK_OC));  // toggle OC2A or OC2B on match to drive GSCLK

// #if (TLC_GSCLK_PIN == PD3 && TLC_GSCLK_PORT == PORTD)
//   TCCR2A = (_BV(WGM21) |   // CTC 
//             _BV(COM2B0));  // toggle OC2B on match -> GSCLK
// #elif (TLC_GSCLK_PIN == PB3 && TLC_GSCLK_PORT == PORTB)
//   TCCR2A = (_BV(WGM21) |   // CTC 
//             _BV(COM2A0));  // toggle OC2A on match -> GSCLK
// #else
// #error "Invalid GSCLK pin configured"
// #endif

  TCCR2B = _BV(CS20);      // No prescaler
//   OCR2B = 1;               // toggle every timer clock cycle -> 4 MHz
  OCR2A = 1;               // toggle every timer clock cycle -> 4 MHz

  TCNT2 = 0;

  // Latch timer
  TCCR1A = (_BV(WGM10));   // Fast PWM 8-bit
  
  /* prescaler
    div 1: CS10 
    div 8: CS11 
    div 64: CS11 | CS10 
    div 256: CS12
    div 1024: CS12 | CS10
  */
  // Fast PWM 8-bit  : 1/4096th of OC2A, div 64 prescaler
//   TCCR1B = (_BV(CS11) | _BV(CS10) | _BV(WGM12));
  // div 8 prescaler
  TCCR1B = (_BV(CS11) | __BV(WGM12));

  // Enable overflow interrupt
  TIMSK1 = _BV(TOIE1);     
  TCNT1 = 0;

//   // Timer 1 - BLANK / XLAT
// #define TLC_PWM_PERIOD    8192
//     TCCR1A = _BV(COM1B1);  // non inverting, output on OC1B, BLANK
//     TCCR1B = _BV(WGM13);   // Phase/freq correct PWM, ICR1 top
//     OCR1A = 1;             // duty factor on OC1A, XLAT is inside BLANK
//     OCR1B = 2;             // duty factor on BLANK (larger than OCR1A (XLAT))
//     ICR1 = TLC_PWM_PERIOD; // see tlc_config.h

//     // Timer 2 - GSCLK
// #define TLC_GSCLK_PERIOD    3
//     TCCR2A = _BV(COM2B1)      // set on BOTTOM, clear on OCR2A (non-inverting),
//                               // output on OC2B
//            | _BV(WGM21)       // Fast pwm with OCR2A top
//            | _BV(WGM20);      // Fast pwm with OCR2A top
//     TCCR2B = _BV(WGM22);      // Fast pwm with OCR2A top
//     OCR2B = 0;                // duty factor (as short a pulse as possible)
//     OCR2A = TLC_GSCLK_PERIOD; // see tlc_config.h
//     TCCR2B |= _BV(CS20);      // no prescale, (start pwm output)
//     TCCR1B |= _BV(CS10);      // no prescale, (start pwm output)

  sei();
}

  // shifts the led data in the given direction
void LedController::shift(uint8_t direction){
  // the leftmost 2 bits determine the direction: 0: left, 1: right, 2: up, 3: down
  // the rightmost 2 bits determines the number of steps: 1-4
  switch(direction & 0xc){
  case 0x0: // shift left
    for(uint8_t col=7; col>0; --col)
      for(uint8_t row=0; row<10; ++row)
        setLed(row, col, getLed(row, col-1));
    for(uint8_t row=0; row<10; ++row)
      setLed(row, 0, 0);
    break;
  case 0x4: // shift right
    for(uint8_t col=0; col<7; ++col)
      for(uint8_t row=0; row<10; ++row)
        setLed(row, col, getLed(row, col+1));
    for(uint8_t row=0; row<10; ++row)
      setLed(row, 7, 0);
    break;
  case 0x8:
    for(uint8_t col=0; col<8; ++col)
      for(uint8_t row=0; row<9; ++row)
        setLed(row, col, getLed(row+1, col));
    for(uint8_t col=0; col<8; ++col)
      setLed(9, col, 0);
    break;
  case 0xc:
    for(uint8_t col=0; col<8; ++col)
      for(uint8_t row=9; row>0; --row)
        setLed(row, col, getLed(row-1, col));
    for(uint8_t col=0; col<8; ++col)
      setLed(0, col, 0);
    break;
  }
}

void LedController::printCharacter(uint8_t* character, uint8_t row, uint8_t col, uint8_t brightness){

  // writing to row (horizontal offset) 0 puts character furthest right
  // row 5 is furthest left (while fitting 5 pixels)

  // row goes from 0-9, col from 0-7
  // font height/width = 8/5 for 6x9 font
  for(int i=0; i<getCharacterHeight(); ++i){
    // font data is 8 bits left adjusted
    uint8_t offset = 8 - getCharacterWidth();
    for(int j=0; j<getCharacterWidth(); ++j){
      // only shift out the relevant bits
      if(character[i] & _BV(j+offset))
        setLed(j+row, i+col, brightness);
      else
        setLed(j+row, i+col, 0x00);
    }
  }
}

void LedController::displayCurrentRow() {
  // shift data out
  uint8_t row = counter.getPosition();
  for(uint8_t i=0;i<LED_CHANNELS;i++)
    shift12bits(led_buffer[row][i]);

  // blank
  TLC_BLANK_PORT |= _BV(TLC_BLANK_PIN);

  // increment active anode pin
  counter.increment();

  // latch
  TLC_XLAT_PORT |= _BV(TLC_XLAT_PIN);
  TLC_XLAT_PORT &= ~_BV(TLC_XLAT_PIN);
    
  // Extra SCLK pulse according to Datasheet p.15:
// The first GS data input cycle after dot correction requires an additional SCLK 
// pulse after the XLAT signal to complete the grayscale update cycle.
  if(doExtraPulse){
    TLC_SCLK_PORT |= _BV(TLC_SCLK_PIN);
    TLC_SCLK_PORT &= ~_BV(TLC_SCLK_PIN);
    doExtraPulse = false;
  }

  // unblank
  TLC_BLANK_PORT &= ~_BV(TLC_BLANK_PIN);
}
