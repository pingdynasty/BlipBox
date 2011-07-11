#include "TouchController.h"
#include "defs.h"
#include "device.h"
#include <avr/interrupt.h> 

// 4 wire resistive touch screen
// read Y coordinate: pin 1 to vcc, pin 2 to gnd, read pin 3 or pin 4
// read X coordinate: pin 3 to vcc, pin 4 to gnd, read pin 1 or pin 2
// pin 1: top
// pin 2: bottom
// pin 3: left
// pin 4: right
// ADC1:        right,  pin 4, XN, Y-Coordinate Reading & Z1 Reading
// ADC2:        top,    pin 2, YP, Z2 Reading
// STANDBY_PIN: bottom, pin 2, YN, Pin Change & Low_Level_Readings & X-Coordinate Reading

// range:
// y 160-860
// x 830-140
// z 300-390

// connect to arduino:
// pin 1 top    to analog 3
// pin 2 bottom to analog 2
// pin 3 left   to analog 1
// pin 4 right  to analog 0

#define TOUCHSCREEN_INPUT                                 PINC
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

// I/O - 4wire
#define XP		                                  PIN_LEFT            // Left Side
#define XN		                                  PIN_RIGHT           // Right Side
#define YP		                                  PIN_TOP             // Upper Side
#define YN		                                  PIN_BOTTOM          // Lower Side
#define ADC1                                              XN            // Y-Coordinate Reading & Z1 Reading
#define ADC2                                              YP            // Z2 Reading
#define STANDBY_PIN                                       YN            // Pin Change & Low_Level_Readings & X-Coordinate Reading

// I/O States
#define STANDBY_CONFIGURATION                             (Pin_Gnd(XP), Pin_Hi_Z(XN), Pin_Hi_Z(YP), Pin_Pullup(YN))
#define X_POS_CONFIGURATION                               (Pin_Gnd(XP), Pin_Vcc(XN), Pin_Hi_Z(YP), Pin_Hi_Z(YN))
#define Y_POS_CONFIGURATION                               (Pin_Hi_Z(XP), Pin_Hi_Z(XN), Pin_Gnd(YP), Pin_Vcc(YN))
#define HI_Z_CONFIGURATION                                (Pin_Hi_Z(XP), Pin_Hi_Z(XN), Pin_Hi_Z(YP), Pin_Hi_Z(YN))
#define Z_1_AND_2_CONFIGURATION                           (Pin_Gnd(XP), Pin_Hi_Z(XN), Pin_Hi_Z(YP), Pin_Vcc(YN))

uint16_t adc_values[7];
uint8_t adc_mode;

#define READ_STANDBY_STATE 1
#define READ_X_POS_STATE   3
#define READ_Y_POS_STATE   5
#define READ_Z1_POS_STATE  7
#define READ_Z2_POS_STATE  9
#define READ_POT_STATE     11

#define START_STATE        READ_STANDBY_STATE
#define END_STATE          12 // one more than last state

void TouchController::init(){
  STANDBY_CONFIGURATION;
//   adc_mode = START_STATE; // leave as 0
  ADMUX = (ADMUX & ~7) | STANDBY_PIN;

  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescalar to 128: 125KHz @ 16MHz
  //  ADCSRA |= (1 << ADPS2) | (1 << ADPS1); // prescalar: 64
  ADMUX |= (1 << REFS0); // Set ADC reference to AVCC
  ADCSRA |= (1 << ADATE); // Set ADC to Free-Running Mode
  ADCSRA |= (1 << ADEN); // Enable ADC
  ADCSRA |= (1 << ADSC); // Start A2D Conversions
  ADCSRA |= (1 << ADIE); // enable ADC interrupt
}

uint16_t TouchController::check(){
  return adc_values[0];
}

uint16_t TouchController::getX(){
  return x;
}

uint16_t TouchController::getY(){
  return adc_values[2];
}

uint16_t TouchController::getZ(){
  return adc_values[5];
}

uint16_t TouchController::getValue(uint8_t index){
  return adc_values[index];
}

ISR(ADC_vect)
{
  switch(adc_mode){
  case READ_STANDBY_STATE : {
    X_POS_CONFIGURATION;
    adc_values[0] = ADCL | ADCH << 8;
    break;
  }
  case READ_X_POS_STATE : {
    Y_POS_CONFIGURATION;
    adc_values[1] = ADCL | ADCH << 8;
    // Set ADC Input Channel for Y-Coordinate Measurement
    ADMUX = (ADMUX & ~7) | ADC1;
    break;
  }
  case READ_Y_POS_STATE : {
    Z_1_AND_2_CONFIGURATION;
    adc_values[2] = ADCL | ADCH << 8;
    break;
  }
  case READ_Z1_POS_STATE : {
    Z_1_AND_2_CONFIGURATION;
    adc_values[3] = ADCL | ADCH << 8;
    // Set ADC Input Channel for Z2 Measurement
    ADMUX = (ADMUX & ~7) | ADC2;
    break;
  }
  case READ_Z2_POS_STATE : {
    STANDBY_CONFIGURATION;
    adc_values[4] = ADCL | ADCH << 8;
    ADMUX = (ADMUX & ~7) | POT_PIN;
    // Calculate Appraisal for R_Touch
    adc_values[5] = ((float)adc_values[4]/adc_values[3]-1)*adc_values[1];
//               R_Touch = ((float)Reading_Z2/Reading_Z1-1) * Readings_X_Pos[i_array];
    break;
  }
  case READ_POT_STATE : {
    STANDBY_CONFIGURATION;
    adc_values[6] = ADCL | ADCH << 8;
    // Set ADC input channel to check for touch
    ADMUX = (ADMUX & ~7) | STANDBY_PIN;
    break;
  }
  }
  if(adc_mode++ == END_STATE)
    adc_mode = START_STATE;
}
