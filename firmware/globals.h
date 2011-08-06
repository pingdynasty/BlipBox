#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stddef.h>
#include <inttypes.h>
#include "BlipBox.h"

#define _BV(bit) (1 << (bit))

void init(void);
unsigned long millis(void);

void* operator new(size_t size);
void operator delete(void* ptr);

extern BlipBox blipbox;

#endif /* _GLOBALS_H_ */
