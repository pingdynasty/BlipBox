/*  Based on code by Alex Leone. Original file header:

    Copyright (c) 2009 by Alex Leone <acleone ~AT~ gmail.com>

    This file is part of the Arduino TLC5940 Library.

    The Arduino TLC5940 Library is free software: you can redistribute it
    and/or modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    The Arduino TLC5940 Library is distributed in the hope that it will be
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Arduino TLC5940 Library.  If not, see
    <http://www.gnu.org/licenses/>. */

#include "LedController.h"

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <wiring.h>
#include "device.h"
#include "globals.h"

/** SS will be set to output as to not interfere with SPI master operation.
    If you have changed the pin-outs and the library doesn't seem to work
    or works intermittently, make sure this pin is set correctly.  This pin
    will not be used by the library other than setting its direction to
    output. */
#define TLC_SS_PIN       PB2
#define TLC_SS_DDR       DDRB
// PB2 is used for BLANK

#define TLC_SPI 1
#define TLC_BITBANG 2

// #ifdef BLIPBOX_V6
#define DATA_TRANSFER_MODE TLC_SPI
// #else
// #define DATA_TRANSFER_MODE TLC_BITBANG
// #endif

#if !(DATA_TRANSFER_MODE == TLC_BITBANG \
 || DATA_TRANSFER_MODE == TLC_SPI)
#error "Invalid DATA_TRANSFER_MODE specified, see DATA_TRANSFER_MODE"
#endif

// // tried 3, 5, 7, 15 (31 seems to get stuck)
// #define TLC_GSCLK_PERIOD 7
// #define TLC_PWM_PERIOD (TLC_GSCLK_PERIOD+1)*2048
// // TLC_GSCLK_PERIOD = TLC_PWM_PERIOD/2048 -1

/** Pulses a pin - high then low. */
#define pulse_pin(port, pin)   port |= _BV(pin); port &= ~_BV(pin)

void tlc_shift8_init(void);
void tlc_shift8(uint8_t byte);

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

void LedController::init(){
  counter.init();
  counter.reset();

      /* Pin Setup */
    TLC_XLAT_DDR |= _BV(TLC_XLAT_PIN);
    TLC_BLANK_DDR |= _BV(TLC_BLANK_PIN);
    TLC_GSCLK_DDR |= _BV(TLC_GSCLK_PIN);
#if TLC_VPRG_ENABLED
    TLC_VPRG_DDR |= _BV(TLC_VPRG_PIN);
    TLC_VPRG_PORT &= ~_BV(TLC_VPRG_PIN);  // grayscale mode (VPRG low)
#endif
#if TLC_XERR_ENABLED
    TLC_XERR_DDR &= ~_BV(TLC_XERR_PIN);   // XERR as input
    TLC_XERR_PORT |= _BV(TLC_XERR_PIN);   // enable pull-up resistor
#endif
    TLC_BLANK_PORT |= _BV(TLC_BLANK_PIN); // leave blank high (until the timers start)

    tlc_shift8_init();

    /* Timer 1 - BLANK / XLAT */
    /* 'old' version
  // Latch timer
  TCCR1A = (_BV(WGM10));   // Fast PWM 8-bit
  // Fast PWM 8-bit  : 1/4096th of OC2A, div 64 prescaler
  TCCR1B = (_BV(CS11) | _BV(CS10) | _BV(WGM12));
  // CS10 | CS11 : clkI/O/64 (From prescaler)
  // WGM12 : mode 4
  // Timer/Counter Mode of Operation: CTC
  // TOP OCR1A
  TIMSK1 = _BV(TOIE1);
  TCNT1 = 0;
    */

    // OC1A is PB1, XLAT
    // OC1B is PB2, BLANK
    // OC2A is PB3, SIN
    // OC2B is PD3, GSCLK
    // TCCR1A and TCCR1B control timer 1
    // TCCR2A and TCCR2B control timer 2
// #define TLC_GSCLK_OC   COM2B0

    // 'old' version for both timers
    /*
  TCCR2A = (_BV(WGM21) |   // CTC (Clear Timer on Compare Match)
            _BV(TLC_GSCLK_OC));  // toggle OC2A or OC2B on match to drive GSCLK
  TCCR2B = _BV(CS20);      // No prescaler
  OCR2B = 64;               // toggle every timer clock cycle -> 4 MHz
  OCR2A = 64;               // toggle every timer clock cycle -> 4 MHz
  TCNT2 = 0;
  TCCR1A = (_BV(WGM10));   // Fast PWM 8-bit
  TCCR1B = (_BV(CS11) | _BV(CS10) | _BV(WGM12));
  TIMSK1 = _BV(TOIE1);     
  TCNT1 = 0;
    */

    // Timer 1 drives TIMER1_OVF_vect, BLANK / XLAT
    TCCR1A = 0;               // OC1A/OC1B disconnected.
    TCCR1B = _BV(WGM13);      // Phase/freq correct PWM, ICR1 top.
//     ICR1 = TLC_PWM_PERIOD;
    ICR1 = (config.tlc_gsclk_period+1)*2048;
    TIFR1 |= _BV(TOV1);
    TIMSK1 = _BV(TOIE1);

    // Timer 2 drives GSCLK
    TCCR2A = _BV(COM2B1)      // set on BOTTOM, clear on OCR2A (non-inverting),
                              // output on OC2B
           | _BV(WGM21)       // Fast pwm with OCR2A top
           | _BV(WGM20);      // Fast pwm with OCR2A top
    TCCR2B = _BV(WGM22);      // Fast pwm with OCR2A top
    OCR2B = 0;                // duty factor (as short a pulse as possible)
    
//     OCR2A = TLC_GSCLK_PERIOD;
    OCR2A = config.tlc_gsclk_period;
    TCCR2B |= _BV(CS20);      // no prescale, (start pwm output)
    TCCR1B |= _BV(CS10);      // no prescale, (start pwm output)
}

/*
    /// need to add these? ->
//     sendBufferData(counter.getPosition());
//     pulse_pin(TLC_XLAT_PORT, TLC_XLAT_PIN);

// TCCR1B = _BV(WGM13) :
// PWM, Phase and Frequency Correct
// ICR1 top
// Update of OCR1x at BOTTOM
// TOV1 Flag Set on BOTTOM

// TCCR1A = _BV(COM1B1) :
// Clear OC1A/OC1B on Compare Match, set OC1A/OC1B at BOTTOM (non-inverting mode)

// #define TLC_PWM_PERIOD    8192
// #define TLC_GSCLK_PERIOD    3

// Output Compare Register B (OCR1B)

// TCCR1A = _BV(COM1B1) enables output on BLANK
// TCCR1A = _BV(COM1A1) enables output on XLAT

    // Enables the Timer1 Overflow interrupt
    // Timer Interrupt Flag Register
    TIFR1 |= _BV(TOV1); 
    // Timer Interrupt Mask Register
    TIMSK1 = _BV(TOIE1);
    // The corresponding Interrupt Vector is executed when the TOV1 Flag, located in TIFR1, is set.
}
*/

void LedController::displayCurrentRow(void){
  // called by Timer 2 overflow ISR
  if(running){
//     running = false;

  // shift, blank, increment, latch, unblank

  // shift data out
  sendBufferData(counter.getPosition());

  // blank
  TLC_BLANK_PORT |= _BV(TLC_BLANK_PIN);

  // increment
  counter.tick();
//   counter.off();
//   counter.increment();
//   counter.on();

  // latch
  TLC_XLAT_PORT |= _BV(TLC_XLAT_PIN);
  TLC_XLAT_PORT &= ~_BV(TLC_XLAT_PIN);

  // unblank
  TLC_BLANK_PORT &= ~_BV(TLC_BLANK_PIN);
//   running = true;
  }
}

void LedController::sendBufferData(uint8_t row){
  // extra clock pulse for grayscale data
  pulse_pin(TLC_SCLK_PORT, TLC_SCLK_PIN);

  //       for(uint8_t i=0;i<LED_CHANNELS;i++)
  //         shift12bits(led_buffer[row][i]);
  for(uint8_t i=0;i<LED_CHANNELS;i++){
    // shift out 24 bits for bytes a and b:
    // aaaaaaaa 0000bbbb bbbb0000
    tlc_shift8(led_buffer[row][i++]);
    tlc_shift8(led_buffer[row][i] >> 4);
    tlc_shift8(led_buffer[row][i] << 4);
  }
}

#if DATA_TRANSFER_MODE == TLC_BITBANG

/** Sets all the bit-bang pins to output */
void tlc_shift8_init(void)
{
    TLC_SIN_DDR |= _BV(TLC_SIN_PIN);   // SIN as output
    TLC_SCLK_DDR |= _BV(TLC_SCLK_PIN); // SCLK as output
    TLC_SCLK_PORT &= ~_BV(TLC_SCLK_PIN);
}

/** Shifts a byte out, MSB first */
void tlc_shift8(uint8_t byte)
{
    for (uint8_t bit = 0x80; bit; bit >>= 1) {
        if (bit & byte) {
            TLC_SIN_PORT |= _BV(TLC_SIN_PIN);
        } else {
            TLC_SIN_PORT &= ~_BV(TLC_SIN_PIN);
        }
        pulse_pin(TLC_SCLK_PORT, TLC_SCLK_PIN);
    }
}

#elif DATA_TRANSFER_MODE == TLC_SPI

/** Initializes the SPI module to double speed (f_osc / 2) */
void tlc_shift8_init(void)
{
    TLC_SIN_DDR    |= _BV(TLC_SIN_PIN);    // SPI MOSI as output
    TLC_SCLK_DDR   |= _BV(TLC_SCLK_PIN);   // SPI SCK as output
    TLC_SS_DDR |= _BV(TLC_SS_PIN); // SPI SS as output

    TLC_SCLK_PORT &= ~_BV(TLC_SCLK_PIN);

    SPSR = _BV(SPI2X); // double speed (f_osc / 2)
    SPCR = _BV(SPE)    // enable SPI
         | _BV(MSTR);  // master mode
}

/** Shifts out a byte, MSB first */
void tlc_shift8(uint8_t byte)
{
    SPDR = byte; // starts transmission
    while (!(SPSR & _BV(SPIF)))
        ; // wait for transmission complete
}

#endif // DATA_TRANSFER_MODE
