#ifndef _GREETING_H_
#define _GREETING_H_

#include "LedController.h"

void rain(LedController& leds);
void fade(LedController& leds);
void bounce(LedController& leds);
void explode(LedController& leds);
void greeting(LedController& leds);

/* void displayEffect(uint8_t type){ */
/*   switch(type){ */
/*   case 0: */
/*     greeting(leds); */
/*     break; */
/*   case 1: */
/*     rain(leds); */
/*     break; */
/*   case 2: */
/*     bounce(leds); */
/*     break; */
/*   case 3: */
/*     fade(leds); */
/*     break; */
/*   case 4: */
/*     explode(leds); */
/*     break; */
/*   } */
/* } */


#endif /* _GREETING_H_ */
