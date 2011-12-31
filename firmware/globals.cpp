#include <stdlib.h>
#include <avr/interrupt.h>
#include "globals.h"

BlipBox blipbox;

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

volatile unsigned long timer0_overflow_count;

ISR(TIMER0_OVF_vect){
  timer0_overflow_count++;
}

unsigned long millis(){
  // timer 0 increments every 64 cycles, and overflows when it reaches
  // 256.  we would calculate the total number of clock cycles, then
  // divide by the number of clock cycles per millisecond, but this
  // overflows too often.
  //return timer0_overflow_count * 64UL * 256UL / (F_CPU / 1000UL);
  // instead find 1/128th the number of clock cycles and divide by
  // 1/128th the number of clock cycles per millisecond
  //return timer0_overflow_count * 64UL * 2UL / (F_CPU / 128000UL);
  return timer0_overflow_count; // the multiply by factor is 1.024, we will be off by 2.4%.
}

void init(){
  // timer 0 is used for millis() and delay()
  timer0_overflow_count = 0;
  // on the ATmega168, timer 0 is also used for fast hardware pwm
  // (using phase-correct PWM would mean that timer 0 overflowed half as often
  // resulting in different millis() behavior on the ATmega8 and ATmega168)
  TCCR0A |= _BV(WGM01) | _BV(WGM00);
  // set timer 0 prescale factor to 64
  TCCR0B |= _BV(CS01) | _BV(CS00);
//   TCCR0B |= _BV(CS02); // prescale factor 256
//   TCCR0B |= _BV(CS02) | _BV(CS00);  // prescale factor 1024, 16 times higher than 64
  // enable timer 0 overflow interrupt
  TIMSK0 |= _BV(TOIE0);
}

// hack/workaround to get round problem with virtual constructors/methods:
// undefined reference to `__cxa_pure_virtual'
// extern "C" {
//   void __cxa_pure_virtual(){}
//   // todo: error handling within function
// }
