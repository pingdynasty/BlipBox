
// The 5-wire touch screen panel works by applying a voltage
// at the corners of the bottom resistive layer and measuring the
// vertical or horizontal resistive network with the wiper

// A measurement of the Y position
// of the pointing device is made by connecting the upper left
// and upper right corners of the resistive layer to V+ and the
// lower left and lower right corners to ground.

// For the horizontal measurement, the upper left corner
// and lower left corner are connected to ground and the upper
// right and lower right corners are connected to V+
#include "TouchController.h"
#include "defs.h"
#include "wiring.h"

// seems to currently work with these connections:
// PC0 - RL / pin 2
// PC1 - RT / pin 1
// PC2 - SG / pin 3
// PC3 - LT / pin 4
// PC4 - LL / pin 5

// connect arduino to touch screen pins
// +5v to 
// RT pin 1 <-> +5
// RL pin 2 <-> digital 4
// SG pin 3 <-> analog 0
// LT pin 4 <-> digital 3
// LL pin 5 <-> GND

#define PIN_RT 1 // right top // pin 1
#define PIN_RL 0 // right low // pin 2
#define PIN_SG 2 // wiper     // pin 3
#define PIN_LT 3 // left top  // pin 4
#define PIN_LL 4 // left low  // pin 5

// adc0 - ul - YMINUS_PIN - (LT pin 4)
// adc1 - ur - XMINUS_PIN - (RT pin 1)
// adc2 - ll - YPLUS_PIN  - (LL pin 5)
// adc3 - lr - XPLUS_PIN  - (RL pin 2)
// adc4 - sense

// I/O
// #define TOUCHSCREEN_INPUT                                 PINC
#define TOUCHSCREEN_OUTPUT                                PORTC
#define TOUCHSCREEN_DDR                                   DDRC

// Macros
#define ClearBit(ADRESS,BIT)                              (ADRESS&=~(1<<BIT))
#define SetBit(ADRESS,BIT)                                (ADRESS|=(1<<BIT))
#define TestBit(ADRESS,BIT)                               ((ADRESS&(1<<BIT)))
#define ToogleBit(ADRESS, BIT)                            ((ADRESS) ^= (1 << (BIT))) 

#define Pin_Pullup(Pin)                                   (SetBit(TOUCHSCREEN_OUTPUT, (Pin)), ClearBit(TOUCHSCREEN_DDR, (Pin)))
#define Pin_Vcc(Pin)                                      (SetBit(TOUCHSCREEN_OUTPUT, (Pin)), SetBit(TOUCHSCREEN_DDR, (Pin)))
#define Pin_Gnd(Pin)                                      (ClearBit(TOUCHSCREEN_OUTPUT, (Pin)), SetBit(TOUCHSCREEN_DDR, (Pin)))
#define Pin_Hi_Z(Pin)                                     (ClearBit(TOUCHSCREEN_DDR, (Pin)), ClearBit(TOUCHSCREEN_OUTPUT, (Pin)))

#define UL		                                  PIN_LT             // Upper Left Corner
#define UR		                                  PIN_RT             // Upper Right Corner
#define LL		                                  PIN_LL             // Lower Left Corner
#define LR		                                  PIN_RL             // Lower Right Corner
#define STANDBY_PIN                                       PIN_SG             // Pin Change & All ADC Readings
#define ADC1                                              STANDBY_PIN

#define STANDBY_CONFIGURATION                             (Pin_Gnd(UL), Pin_Hi_Z(UR), Pin_Hi_Z(LL), Pin_Hi_Z(LR), Pin_Pullup(STANDBY_PIN))
#define X_POS_CONFIGURATION                               (Pin_Gnd(UL), Pin_Vcc(UR), Pin_Gnd(LL), Pin_Vcc(LR), Pin_Hi_Z(STANDBY_PIN))
#define Y_POS_CONFIGURATION                               (Pin_Gnd(UL), Pin_Gnd(UR), Pin_Vcc(LL), Pin_Vcc(LR), Pin_Hi_Z(STANDBY_PIN))
#define HI_Z_CONFIGURATION                                (Pin_Hi_Z(UL), Pin_Hi_Z(UR), Pin_Hi_Z(LL), Pin_Hi_Z(LR), Pin_Hi_Z(STANDBY_PIN))

void TouchController::init(){
//   pinMode(PIN_RL, OUTPUT);
//   pinMode(PIN_LT, OUTPUT);
// //   pinMode(PIN_RL, INPUT); // enables pull-up resistor
// //   pinMode(PIN_LT, INPUT); // enables pull-up resistor
//   pinMode(PIN_RT, OUTPUT);
  STANDBY_CONFIGURATION;
}

uint16_t TouchController::check(){
  STANDBY_CONFIGURATION;
#ifdef SENSE_DELAY
  delay(SENSE_DELAY);
#endif // SENSE_DELAY
  tval = analogRead(PIN_SG);
  return tval;
}

uint16_t TouchController::readX(){
  X_POS_CONFIGURATION;
#ifdef SENSE_DELAY
  delay(SENSE_DELAY);
#endif // SENSE_DELAY
  xval = analogRead(PIN_SG);
  return xval;
}

uint16_t TouchController::readY(){
  Y_POS_CONFIGURATION;
#ifdef SENSE_DELAY
  delay(SENSE_DELAY);
#endif // SENSE_DELAY
  yval = analogRead(PIN_SG);
  return yval;
}
