#include "io.h"

uint8_t TCCR0A;
uint8_t TCCR0B;
uint8_t WGM01;
uint8_t WGM00;
uint8_t CS01;
uint8_t CS00;
uint8_t TIMSK0;
uint8_t TOIE0;
uint8_t DDRB, DDRC, DDRD;
uint8_t PORTB, PORTC, PORTD;
uint8_t PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7;
uint8_t PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7;
uint8_t PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7;
uint8_t UDR0 = 0;
