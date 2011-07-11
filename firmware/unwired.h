#ifndef UNWIRED_H
#define UNWIRED_H

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <avr/interrupt.h>

#include <WProgram.h>

/* #include <wiring.h> */

/* #ifdef abs */
/* #undef abs */
/* #endif */
/* #define abs(x) ((x)>0?(x):-(x)) */

#ifndef __AVR_ATmega168__
#error "hey!"
#define __AVR_ATmega168__
#endif

#ifdef __cplusplus
extern "C"{
#endif

unsigned long millis(void);
void unwiredinit(void);
void beginSerial(long baud);
void serialWrite(unsigned char c);

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define EXTERNAL_INT_0 0
#define EXTERNAL_INT_1 1

#define EXTERNAL_NUM_INTERRUPTS 2

typedef void (*voidFuncPtr)(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // UNWIRED_H
