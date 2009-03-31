#ifndef _DEVICE_H_
#define _DEVICE_H_

/* Header file containing device-specific defines */

#define BLIPBOX_P4

#if defined BLIPBOX_P2

#define TLC_DDR        DDRB
#define TLC_PORT       PORTB
#define SCLK_PIN       PB0
#define XLAT_PIN       PB1
#define BLANK_PIN      PB2
#define GSCLK_PIN      PB3
#define VPRG_PIN       PB4
#define SIN_PIN        PB5

#define PIN_RT 1 // right top // pin 1
#define PIN_RL 0 // right low // pin 2
#define PIN_SG 2 // wiper     // pin 3
#define PIN_LT 3 // left top  // pin 4
#define PIN_LL 4 // left low  // pin 5

#elif defined BLIPBOX_P4

#define TLC_DDR        DDRB
#define TLC_PORT       PORTB
#define SCLK_PIN       PB0
#define XLAT_PIN       PB1
#define BLANK_PIN      PB2
#define GSCLK_PIN      PB3
#define VPRG_PIN       PB4
#define SIN_PIN        PB5

#define PIN_RT 0 // right top // pin 1
#define PIN_RL 1 // right low // pin 2
#define PIN_SG 2 // wiper     // pin 3
#define PIN_LT 3 // left top  // pin 4
#define PIN_LL 4 // left low  // pin 5

#endif /* BLIPBOX_xx */


#endif /* _DEVICE_H_ */
