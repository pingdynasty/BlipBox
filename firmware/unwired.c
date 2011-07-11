#include "unwired.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned long timer0_overflow_count;

SIGNAL(SIG_OVERFLOW0)
{
	timer0_overflow_count++;
}

unsigned long millis()
{
	// timer 0 increments every 64 cycles, and overflows when it reaches
	// 256.  we would calculate the total number of clock cycles, then
	// divide by the number of clock cycles per millisecond, but this
	// overflows too often.
	//return timer0_overflow_count * 64UL * 256UL / (F_CPU / 1000UL);
	
	// instead find 1/128th the number of clock cycles and divide by
	// 1/128th the number of clock cycles per millisecond
	return timer0_overflow_count * 64UL * 2UL / (F_CPU / 128000UL);
}

void unwiredinit()
{
	// this needs to be called before setup() or some functions won't
	// work there
	sei();
	
	// timer 0 is used for millis() and delay()
	timer0_overflow_count = 0;
	// on the ATmega168, timer 0 is also used for fast hardware pwm
	// (using phase-correct PWM would mean that timer 0 overflowed half as often
	// resulting in different millis() behavior on the ATmega8 and ATmega168)
#if defined(__AVR_ATmega168__)
	sbi(TCCR0A, WGM01);
	sbi(TCCR0A, WGM00);
#endif  
	// set timer 0 prescale factor to 64
#if defined(__AVR_ATmega168__)
	sbi(TCCR0B, CS01);
	sbi(TCCR0B, CS00);
#else
	sbi(TCCR0, CS01);
	sbi(TCCR0, CS00);
#endif
	// enable timer 0 overflow interrupt
#if defined(__AVR_ATmega168__)
	sbi(TIMSK0, TOIE0);
#else
	sbi(TIMSK, TOIE0);
#endif

	// timers 1 and 2 are used for phase-correct hardware pwm
	// this is better for motors as it ensures an even waveform
	// note, however, that fast pwm mode can achieve a frequency of up
	// 8 MHz (with a 16 MHz clock) at 50% duty cycle

	// set timer 1 prescale factor to 64
	sbi(TCCR1B, CS11);
	sbi(TCCR1B, CS10);
	// put timer 1 in 8-bit phase correct pwm mode
	sbi(TCCR1A, WGM10);

	// set timer 2 prescale factor to 64
#if defined(__AVR_ATmega168__)
	sbi(TCCR2B, CS22);
#else
	sbi(TCCR2, CS22);
#endif
	// configure timer 2 for phase correct pwm (8-bit)
#if defined(__AVR_ATmega168__)
	sbi(TCCR2A, WGM20);
#else
	sbi(TCCR2, WGM20);
#endif

	// set a2d prescale factor to 128
	// 16 MHz / 128 = 125 KHz, inside the desired 50-200 KHz range.
	// XXX: this will not work properly for other clock speeds, and
	// this code should use F_CPU to determine the prescale factor.
	sbi(ADCSRA, ADPS2);
	sbi(ADCSRA, ADPS1);
	sbi(ADCSRA, ADPS0);

	// enable a2d conversions
	sbi(ADCSRA, ADEN);

	// the bootloader connects pins 0 and 1 to the USART; disconnect them
	// here so they can be used as normal digital i/o; they will be
	// reconnected in Serial.begin()
#if defined(__AVR_ATmega168__)
	UCSR0B = 0;
#else
	UCSRB = 0;
#endif
}
