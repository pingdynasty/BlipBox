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
  extern uint8_t PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7;
  extern uint8_t PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7;
  extern uint8_t PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7;
  extern uint8_t UDR0;

#ifdef __cplusplus
}
#endif

#endif /* _AVR_IO_H_ */
