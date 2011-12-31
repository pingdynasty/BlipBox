#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "defs.h"
#include <avr/io.h>

/* Header file containing device-specific defines */

#if defined BLIPBOX_P4

#define TLC_SCLK_DDR   DDRB
#define TLC_SCLK_PORT  PORTB
#define TLC_SCLK_PIN   PORTB0

#define TLC_XLAT_DDR   DDRB
#define TLC_XLAT_PORT  PORTB
#define TLC_XLAT_PIN   PORTB1

#define TLC_BLANK_DDR  DDRB
#define TLC_BLANK_PORT PORTB
#define TLC_BLANK_PIN  PORTB2

#define TLC_GSCLK_DDR  DDRB
#define TLC_GSCLK_PORT PORTB
#define TLC_GSCLK_PIN  PORTB3

/* #define TLC_VPRG_DDR   DDRB */
/* #define TLC_VPRG_PORT  PORTB */
/* #define TLC_VPRG_PIN   PB4 */

#define TLC_SIN_DDR    DDRB
#define TLC_SIN_PORT   PORTB
#define TLC_SIN_PIN    PORTB5

#define TLC_GSCLK_OC   COM2A0

#define LED_ROW_1      3
#define LED_ROW_2      7
#define LED_ROW_3      6
#define LED_ROW_4      5
#define LED_ROW_5      4

#define LED_ROW_1_PIN      PORTD3
#define LED_ROW_1_DDR      DDRD
#define LED_ROW_1_PORT     PORTD

#define LED_ROW_2_PIN      PORTD7
#define LED_ROW_2_DDR      DDRD
#define LED_ROW_2_PORT     PORTD

#define LED_ROW_3_PIN      PORTD6
#define LED_ROW_3_DDR      DDRD
#define LED_ROW_3_PORT     PORTD

#define LED_ROW_4_PIN      PORTD5
#define LED_ROW_4_DDR      DDRD
#define LED_ROW_4_PORT     PORTD

#define LED_ROW_5_PIN      PORTD4
#define LED_ROW_5_DDR      DDRD
#define LED_ROW_5_PORT     PORTD

#define PIN_RT 0 // right top // pin 1
#define PIN_RL 1 // right low // pin 2
#define PIN_SG 2 // wiper     // pin 3
#define PIN_LT 3 // left top  // pin 4
#define PIN_LL 4 // left low  // pin 5

// pins
#define POT_PIN 5
#define BUTTON1_PIN 2

#elif defined BLIPBOX_V6

#define TLC_DCPRG_ENABLED 0
#define TLC_VPRG_ENABLED  0
#define TLC_XERR_ENABLED  0

#define TLC_SCLK_DDR   DDRB
#define TLC_SCLK_PORT  PORTB
#define TLC_SCLK_PIN   PORTB5

#define TLC_XLAT_DDR   DDRB
#define TLC_XLAT_PORT  PORTB
#define TLC_XLAT_PIN   PORTB1

#define TLC_BLANK_DDR  DDRB
#define TLC_BLANK_PORT PORTB
#define TLC_BLANK_PIN  PORTB2

#define TLC_SIN_DDR    DDRB
#define TLC_SIN_PORT   PORTB
#define TLC_SIN_PIN    PORTB3

#define TLC_GSCLK_DDR  DDRD
#define TLC_GSCLK_PORT PORTD
#define TLC_GSCLK_PIN  PORTD3

#define TLC_GSCLK_OC   COM2B0

#define LED_ROW_1      2
#define LED_ROW_2      12
#define LED_ROW_3      8
#define LED_ROW_4      7
#define LED_ROW_5      4

#define LED_ROW_1_PIN      PORTD2
#define LED_ROW_1_PORT     PORTD
#define LED_ROW_1_DDR      DDRD

#define LED_ROW_2_PIN      PORTB4
#define LED_ROW_2_PORT     PORTB
#define LED_ROW_2_DDR      DDRB

#define LED_ROW_3_PIN      PORTB0
#define LED_ROW_3_PORT     PORTB
#define LED_ROW_3_DDR      DDRB

#define LED_ROW_4_PIN      PORTD7
#define LED_ROW_4_PORT     PORTD
#define LED_ROW_4_DDR      DDRD

#define LED_ROW_5_PIN      PORTD4
#define LED_ROW_5_PORT     PORTD
#define LED_ROW_5_DDR      DDRD

#define PIN_RT             0 // right top
#define PIN_RL             1 // right low
#define PIN_SG             2 // wiper    
#define PIN_LT             3 // left top 
#define PIN_LL             4 // left low 

// external connections
#define POT_PIN     5 // PC5 // arduino analog pin
#define BUTTON1_PIN 5 // PORTD5 // arduino digital pin
#define BUTTON2_PIN 5 // PD5 // arduino digital pin

#elif defined BLIPBOX_V8

#define TLC_DCPRG_ENABLED 0
#define TLC_VPRG_ENABLED  0
#define TLC_XERR_ENABLED  0

#define TLC_SCLK_DDR   DDRB
#define TLC_SCLK_PORT  PORTB
#define TLC_SCLK_PIN   PORTB5

#define TLC_XLAT_DDR   DDRB
#define TLC_XLAT_PORT  PORTB
#define TLC_XLAT_PIN   PORTB1

#define TLC_BLANK_DDR  DDRB
#define TLC_BLANK_PORT PORTB
#define TLC_BLANK_PIN  PORTB2

#define TLC_SIN_DDR    DDRB
#define TLC_SIN_PORT   PORTB
#define TLC_SIN_PIN    PORTB3

#define TLC_GSCLK_DDR  DDRD
#define TLC_GSCLK_PORT PORTD
#define TLC_GSCLK_PIN  PORTD3

#define TLC_GSCLK_OC   COM2B0

#define MIC_DATA_PIN   PORTD2
#define MIC_DATA_DDR   DDRD
#define MIC_DATA_PORT  PORTD
#define MIC_CLOCK_PIN  PORTD4
#define MIC_CLOCK_DDR  DDRD
#define MIC_CLOCK_PORT PORTD

#define PIN_RT             0 // right top
#define PIN_RL             1 // right low
#define PIN_SG             2 // wiper    
#define PIN_LT             3 // left top 
#define PIN_LL             4 // left low 

// external connections
#define POT_PIN     5 // PC5 // arduino analog pin
#define BUTTON1_PIN 5 // PD5 // arduino digital pin
#define BUTTON2_PIN 5 // PD5 // arduino digital pin

#elif defined BLIPBOX_V9

#define TLC_DCPRG_ENABLED 1
#define TLC_VPRG_ENABLED  1
#define TLC_XERR_ENABLED  0

#define TLC_DCPRG_DDR  DDRB
#define TLC_DCPRG_PORT PORTB
#define TLC_DCPRG_PIN  PORTB4

#define TLC_VPRG_DDR   DDRB
#define TLC_VPRG_PORT  PORTB
#define TLC_VPRG_PIN   PORTB0

#define TLC_SCLK_DDR   DDRB
#define TLC_SCLK_PORT  PORTB
#define TLC_SCLK_PIN   PORTB5

#define TLC_XLAT_DDR   DDRB
#define TLC_XLAT_PORT  PORTB
#define TLC_XLAT_PIN   PORTB1

#define TLC_BLANK_DDR  DDRB
#define TLC_BLANK_PORT PORTB
#define TLC_BLANK_PIN  PORTB2

#define TLC_SIN_DDR    DDRB
#define TLC_SIN_PORT   PORTB
#define TLC_SIN_PIN    PORTB3

#define TLC_GSCLK_DDR  DDRD
#define TLC_GSCLK_PORT PORTD
#define TLC_GSCLK_PIN  PORTD3

#define TLC_GSCLK_OC   COM2B0

#define LED_ROW_1      4
#define LED_ROW_2      2
#define LED_ROW_3      7
#define LED_ROW_4      6
#define LED_ROW_5      5

#define LED_ROW_1_PIN      PORTD2
#define LED_ROW_1_PORT     PORTD
#define LED_ROW_1_DDR      DDRD

#define LED_ROW_2_PIN      PORTD7
#define LED_ROW_2_PORT     PORTD
#define LED_ROW_2_DDR      DDRD

#define LED_ROW_3_PIN      PORTD6
#define LED_ROW_3_PORT     PORTD
#define LED_ROW_3_DDR      DDRD

#define LED_ROW_4_PIN      PORTD5
#define LED_ROW_4_PORT     PORTD
#define LED_ROW_4_DDR      DDRD

#define LED_ROW_5_PIN      PORTD4
#define LED_ROW_5_PORT     PORTD
#define LED_ROW_5_DDR      DDRD

#define PIN_RT             0 // right top
#define PIN_RL             1 // right low
#define PIN_SG             2 // wiper    
#define PIN_LT             3 // left top 
#define PIN_LL             4 // left low 

// external connections
#define POT_PIN     5  // PC5 // arduino analog pin
#define BUTTON1_PIN 8  // PB0 // arduino digital pin
#define BUTTON2_PIN 12 // PB4 // arduino digital pin

#elif defined MONOTAR_P1  /* end if BLIPBOX_xx */

#define POT_PIN            1
#define PIN_TOP            2
#define PIN_BOTTOM         3
#define PIN_LEFT           4
#define PIN_RIGHT          5

#else
#error "No valid device defined"
#endif

/*
 Arduino digital pin 0-7  = PORTD, PD0-7
 Arduino digital pin 8-13 = PORTB, PB0-5
 Arduino analog pin  0-5  = PORTC, PC0-5 
*/


#endif /* _DEVICE_H_ */
