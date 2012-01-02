#ifndef _AVR_IO_H_
#define _AVR_IO_H_

#include <inttypes.h>

#define _BV(bit) (1 << (bit))

#ifdef __cplusplus
extern "C" {
#endif

  extern uint8_t TCCR0A, WGM01, WGM00, TCCR0B, CS01, CS00, TIMSK0, TOIE0;
  extern uint8_t DDRB, DDRC, DDRD;
  extern uint8_t PORTB, PORTC, PORTD;
  extern uint8_t PORTB0, PORTB1, PORTB2, PORTB3, PORTB4, PORTB5, PORTB6, PORTB7;
  extern uint8_t PORTC0, PORTC1, PORTC2, PORTC3, PORTC4, PORTC5, PORTC6, PORTC7;
  extern uint8_t PORTD0, PORTD1, PORTD2, PORTD3, PORTD4, PORTD5, PORTD6, PORTD7;
  extern uint8_t UDR0;

#ifdef __cplusplus
}
#endif

#endif /* _AVR_IO_H_ */
