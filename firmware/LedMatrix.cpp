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

#include <avr/io.h>
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

#if !(TLC_DATA_TRANSFER_MODE == TLC_BITBANG \
 || TLC_DATA_TRANSFER_MODE == TLC_SPI)
#error "Invalid TLC_DATA_TRANSFER_MODE specified, see TLC_DATA_TRANSFER_MODE"
#endif

// // tried 3, 5, 7, 15 (31 seems to get stuck)
// #define TLC_GSCLK_PERIOD 7
// #define TLC_PWM_PERIOD (TLC_GSCLK_PERIOD+1)*2048
// // TLC_GSCLK_PERIOD = TLC_PWM_PERIOD/2048 -1

/** Pulses a pin - high then low. */
#define pulse_pin(port, pin)   port |= _BV(pin); port &= ~_BV(pin)

void tlc_shift8_init(void);
void tlc_shift8(uint8_t data);

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

void LedController::init(){
  back_buffer = buf1;
  counter.init();

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

#if TLC_DCPRG_ENABLED
  TLC_DCPRG_DDR |= _BV(TLC_DCPRG_PIN);
  //   TLC_DCPRG_PORT |= _BV(TLC_DCPRG_PIN); // DCPRG high
  TLC_DCPRG_PORT &= ~_BV(TLC_DCPRG_PIN); // DCPRG low
#endif

  tlc_shift8_init();

  // OC1A is PB1, XLAT
  // OC1B is PB2, BLANK
  // OC2A is PB3, SIN
  // OC2B is PD3, GSCLK
  // TCCR1A and TCCR1B control timer 1
  // TCCR2A and TCCR2B control timer 2

  // Timer 1 drives TIMER1_OVF_vect, BLANK / XLAT
  TCCR1A = 0;               // OC1A/OC1B disconnected.
  TCCR1B = _BV(WGM13);      // Phase/freq correct PWM, ICR1 top.
  //     ICR1 = TLC_PWM_PERIOD;
  ICR1 = (blipbox.config.tlc_gsclk_period+1)*2048;
  TIFR1 |= _BV(TOV1);
  TIMSK1 = _BV(TOIE1);

  // Timer 2 drives GSCLK
  TCCR2A = _BV(COM2B1)      // set on BOTTOM, clear on OCR2A (non-inverting),
    // output on OC2B
    | _BV(WGM21)       // Fast pwm with OCR2A top
    | _BV(WGM20);      // Fast pwm with OCR2A top
  TCCR2B = _BV(WGM22);      // Fast pwm with OCR2A top
  OCR2B = 0;                // duty factor (as short a pulse as possible)
    
  OCR2A = blipbox.config.tlc_gsclk_period;
  TCCR2B |= _BV(CS20);      // no prescale, (start pwm output)
  TCCR1B |= _BV(CS10);      // no prescale, (start pwm output)
}

void LedController::flip(){
  uint8_t* front_buffer = flipped ? buf2 : buf1;
  back_buffer = flipped ? buf1 : buf2;  
  // memcpy(to, from, bytes)
  memcpy(back_buffer, front_buffer, LED_BUFFER_LENGTH);
  flipped = !flipped;
}

void LedController::displayCurrentRow(void){
  // called by Timer 2 overflow ISR
  // sequence: shift, blank, increment, latch, unblank
  // shift data out
  sendBufferData(counter.getPosition());
  // blank
  TLC_BLANK_PORT |= _BV(TLC_BLANK_PIN);
  // increment
  counter.tick();
  // latch
  TLC_XLAT_PORT |= _BV(TLC_XLAT_PIN);
  TLC_XLAT_PORT &= ~_BV(TLC_XLAT_PIN);
  // unblank
  TLC_BLANK_PORT &= ~_BV(TLC_BLANK_PIN);
}

void LedController::sendBufferData(uint8_t row){
  // extra clock pulse for grayscale data
  pulse_pin(TLC_SCLK_PORT, TLC_SCLK_PIN);

  uint8_t* ptr = flipped ? buf2 : buf1;
  ptr += row * LED_CHANNELS;

  // shift out 24 bits for bytes a and b:
  // aaaaaaaa 0000bbbb bbbb0000
  for(uint8_t i=0;i<LED_CHANNELS;i++){
    tlc_shift8(ptr[i++]);
    tlc_shift8(ptr[i]>>4);
    tlc_shift8(ptr[i]<<4);
  }
}

#if TLC_DATA_TRANSFER_MODE == TLC_BITBANG

/** Sets all the bit-bang pins to output */
void tlc_shift8_init(void){
    TLC_SIN_DDR |= _BV(TLC_SIN_PIN);   // SIN as output
    TLC_SCLK_DDR |= _BV(TLC_SCLK_PIN); // SCLK as output
    TLC_SCLK_PORT &= ~_BV(TLC_SCLK_PIN);
}

/** Shifts a byte out, MSB first */
void tlc_shift8(uint8_t data){
    for (uint8_t bit = 0x80; bit; bit >>= 1) {
        if (bit & data) {
            TLC_SIN_PORT |= _BV(TLC_SIN_PIN);
        } else {
            TLC_SIN_PORT &= ~_BV(TLC_SIN_PIN);
        }
        pulse_pin(TLC_SCLK_PORT, TLC_SCLK_PIN);
    }
}

#elif TLC_DATA_TRANSFER_MODE == TLC_SPI

/** Initializes the SPI module to double speed (f_osc / 2) */
void tlc_shift8_init(void){
    TLC_SIN_DDR    |= _BV(TLC_SIN_PIN);    // SPI MOSI as output
    TLC_SCLK_DDR   |= _BV(TLC_SCLK_PIN);   // SPI SCK as output
    TLC_SS_DDR |= _BV(TLC_SS_PIN); // SPI SS as output

    TLC_SCLK_PORT &= ~_BV(TLC_SCLK_PIN);

    SPSR = _BV(SPI2X); // double speed (f_osc / 2)
    SPCR = _BV(SPE)    // enable SPI
         | _BV(MSTR);  // master mode
}

/** Shifts out a byte, MSB first */
void tlc_shift8(uint8_t data){
    SPDR = data; // starts transmission
    while (!(SPSR & _BV(SPIF)))
        ; // wait for transmission complete
}

#endif // TLC_DATA_TRANSFER_MODE
