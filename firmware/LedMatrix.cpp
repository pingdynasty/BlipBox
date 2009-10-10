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

#ifdef BLIPBOX_V6
#define DATA_TRANSFER_MODE TLC_SPI
#else
#define DATA_TRANSFER_MODE TLC_BITBANG
#endif

#if !(DATA_TRANSFER_MODE == TLC_BITBANG \
 || DATA_TRANSFER_MODE == TLC_SPI)
#error "Invalid DATA_TRANSFER_MODE specified, see DATA_TRANSFER_MODE"
#endif

#define TLC_PWM_PERIOD    8192
#define TLC_GSCLK_PERIOD    3

/** Enables the Timer1 Overflow interrupt, which will fire after an XLAT
    pulse */
#define set_XLAT_interrupt()    TIFR1 |= _BV(TOV1); TIMSK1 = _BV(TOIE1)
/** Disables any Timer1 interrupts */
#define clear_XLAT_interrupt()  TIMSK1 = 0

/** Enables the output of XLAT pulses */
#define enable_XLAT_pulses()    TCCR1A = _BV(COM1A1) | _BV(COM1B1)
/** Disables the output of XLAT pulses */
#define disable_XLAT_pulses()   TCCR1A = _BV(COM1B1)

/** Pulses a pin - high then low. */
#define pulse_pin(port, pin)   port |= _BV(pin); port &= ~_BV(pin)

/** This will be true (!= 0) if update was just called and the data has not
    been latched in yet. */
volatile uint8_t tlc_needXLAT;

/** Some of the extened library will need to be called after a successful
    update. */
// volatile void (*tlc_onUpdateFinished)(void);

static uint8_t firstGSInput;

void tlc_shift8_init(void);
void tlc_shift8(uint8_t byte);

volatile uint8_t isShifting;

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
    update();
    disable_XLAT_pulses();
    clear_XLAT_interrupt();
    tlc_needXLAT = 0;
    pulse_pin(TLC_XLAT_PORT, TLC_XLAT_PIN);

    /* Timer Setup */

    /* Timer 1 - BLANK / XLAT */
    TCCR1A = _BV(COM1B1);  // non inverting, output on OC1B, BLANK
    TCCR1B = _BV(WGM13);   // Phase/freq correct PWM, ICR1 top
    OCR1A = 1;             // duty factor on OC1A, XLAT is inside BLANK
    OCR1B = 2;             // duty factor on BLANK (larger than OCR1A (XLAT))
    ICR1 = TLC_PWM_PERIOD; // see tlc_config.h

    /* Timer 2 - GSCLK */
#ifdef TLC_ATMEGA_8_H
    TCCR2  = _BV(COM20)       // set on BOTTOM, clear on OCR2A (non-inverting),
           | _BV(WGM21);      // output on OC2B, CTC mode with OCR2 top
    OCR2   = TLC_GSCLK_PERIOD / 2; // see tlc_config.h
    TCCR2 |= _BV(CS20);       // no prescale, (start pwm output)
#else
    TCCR2A = _BV(COM2B1)      // set on BOTTOM, clear on OCR2A (non-inverting),
                              // output on OC2B
           | _BV(WGM21)       // Fast pwm with OCR2A top
           | _BV(WGM20);      // Fast pwm with OCR2A top
    TCCR2B = _BV(WGM22);      // Fast pwm with OCR2A top
    OCR2B = 0;                // duty factor (as short a pulse as possible)
    OCR2A = TLC_GSCLK_PERIOD; // see tlc_config.h
    TCCR2B |= _BV(CS20);      // no prescale, (start pwm output)
#endif
    TCCR1B |= _BV(CS10);      // no prescale, (start pwm output)
    update();
}

/** Shifts in the data from the grayscale data array, #tlc_GSData.
    If data has already been shifted in this grayscale cycle, another call to
    update() will immediately return 1 without shifting in the new data.  To
    ensure that a call to update() does shift in new data, use
    \code while(Tlc.update()); \endcode
    or
    \code while(tlc_needXLAT); \endcode
    \returns 1 if there is data waiting to be latched, 0 if data was
             successfully shifted in */
uint8_t LedController::update(void)
{
    if (tlc_needXLAT) {
        return 1;
    }
    disable_XLAT_pulses();

    sendBufferData(counter.getPosition());

    tlc_needXLAT = 1;
    enable_XLAT_pulses();
    set_XLAT_interrupt();
    return 0;
}

void LedController::displayCurrentRow(void)
{
  if(!isShifting) {

    disable_XLAT_pulses();
    isShifting = 1;
    sei();

    counter.off();
    // increment active anode pin
    counter.increment();
    counter.on();

    // shift data out
    sendBufferData(counter.getPosition());

    enable_XLAT_pulses();
    isShifting = 0;

    tlc_needXLAT = 0; // assuming this has been called from XLAT interrupt
  }

}

void LedController::sendBufferData(uint8_t row){
  if (firstGSInput) {
    // adds an extra SCLK pulse unless we've just set dot-correction data
    firstGSInput = 0;
  } else {
    pulse_pin(TLC_SCLK_PORT, TLC_SCLK_PIN);
  }

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
