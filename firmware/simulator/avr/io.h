#ifndef _AVR_IO_H_
#define _AVR_IO_H_

#include <inttypes.h>

#define _BV(bit) (1 << (bit))

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t TCCR0A, WGM01, WGM00, TCCR0B, CS01, CS00, TIMSK0, TOIE0;
extern uint8_t DDRD, PORTD, PD2, PD3, PD4, PD5, PD6, PD7;
extern uint8_t UDR0;

#ifdef __cplusplus
}
#endif

#endif /* _AVR_IO_H_ */
