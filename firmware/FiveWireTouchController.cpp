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
#include "device.h"
#include <avr/interrupt.h> 

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

#include "device.h"

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
#define ClearBit(ADDRESS,BIT)                              (ADDRESS&=~(1<<BIT))
#define SetBit(ADDRESS,BIT)                                (ADDRESS|=(1<<BIT))

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

uint16_t adc_values[4];
uint8_t adc_mode;
#define READ_STANDBY_STATE 1
#define READ_X_POS_STATE   3
#define READ_Y_POS_STATE   5

#define START_STATE        READ_STANDBY_STATE
#define END_STATE          6 // one more than last state

void TouchController::init(){
//   pinMode(PIN_RL, OUTPUT);
//   pinMode(PIN_LT, OUTPUT);
// //   pinMode(PIN_RL, INPUT); // enables pull-up resistor
// //   pinMode(PIN_LT, INPUT); // enables pull-up resistor
//   pinMode(PIN_RT, OUTPUT);

  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescalar to 128: 125KHz @ 16MHz (divided by 31: 4KHz)
  //  ADCSRA |= (1 << ADPS2) | (1 << ADPS1); // prescalar: 64
  ADMUX |= (1 << REFS0); // Set ADC reference to AVCC
  ADCSRA |= (1 << ADATE); // Set ADC to Free-Running Mode
  ADCSRA |= (1 << ADEN); // Enable ADC
  ADCSRA |= (1 << ADSC); // Start A2D Conversions
  ADCSRA |= (1 << ADIE); // enable ADC interrupt


  STANDBY_CONFIGURATION;
//   adc_mode = READ_STANDBY; // leave as zero  
  ADMUX = (ADMUX & ~7) | PIN_SG; // set ADC to read SG pin
}

uint16_t TouchController::getX(){
  return adc_values[1];
}

uint16_t TouchController::getY(){
  return adc_values[2];
}

uint16_t TouchController::getZ(){
  return adc_values[0];
}

uint16_t TouchController::check(){
  // same as getZ()
  return adc_values[0];
}

uint16_t TouchController::getValue(uint8_t index){
  return adc_values[index];
}

ISR(ADC_vect)
{
  switch(adc_mode){
  case READ_STANDBY_STATE:{
    X_POS_CONFIGURATION;
    adc_values[0] = ADCL;
    adc_values[0] |= ADCH << 8;
//     ADMUX = (ADMUX & ~7) | POT_PIN;
//     adc_mode = X_POS_CONFIGURATION;
    break;
  }
  case READ_X_POS_STATE:{
    Y_POS_CONFIGURATION;
    adc_values[1] = ADCL;
    adc_values[1] |= ADCH << 8;
//     ADMUX = (ADMUX & ~7) | POT_PIN;
//     adc_mode = Y_POS_CONFIGURATION;
    break;
  }
  case READ_Y_POS_STATE:{
    STANDBY_CONFIGURATION;
    adc_values[2] = ADCL;
    adc_values[2] |= ADCH << 8;
//     ADMUX = (ADMUX & ~7) | POT_PIN;
//     adc_mode = STANDBY_CONFIGURATION;
    break;
  }
//   default:{
//     // happens on every even adc_mode, i.e. every other time
//     adc_values[3] = ADCL;
//     adc_values[3] |= ADCH << 8;
//     ADMUX = (ADMUX & ~7) | PIN_SG;
//     break;
//   }
  }
  if(++adc_mode == END_STATE)
    adc_mode = 0;
//   uint8_t curchan = ADMUX & 7;

//   adc_values[oldchan] = ADCL;
//   adc_values[oldchan] |= ADCH << 8;
// #ifdef FILTER_BITS
//   adc_values[oldchan] &= FILTER_BITS;
// #endif
//   oldchan = curchan;

//   if(++curchan == ADC_CHANNELS)
//     curchan = 0;

//   ADMUX = (ADMUX & ~7) | curchan;
}
