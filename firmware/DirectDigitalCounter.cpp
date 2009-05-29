#include "DirectDigitalCounter.h"
#include <stdlib.h>
#include <avr/io.h>
#include <wiring.h>
#include "device.h"
#include "pins_arduino.h"

#define CNT_PINS 5 // only 5, really, but we map from 10 values

// note: make sure that CNT_PINS is the right size!
// uint8_t pins[] = { LED_ROW_1, LED_ROW_2, LED_ROW_3, LED_ROW_4, LED_ROW_5 };
uint8_t pins[CNT_PINS] = { LED_ROW_1, LED_ROW_2, LED_ROW_3, LED_ROW_4, LED_ROW_5 };
uint8_t bits[CNT_PINS];
uint8_t ports[CNT_PINS];

void Counter::init(){
//   bits[0] = digitalPinToBitMask(LED_ROW_1);
//   bits[1] = digitalPinToBitMask(LED_ROW_2);
//   bits[2] = digitalPinToBitMask(LED_ROW_3);
//   bits[3] = digitalPinToBitMask(LED_ROW_4);
//   bits[4] = digitalPinToBitMask(LED_ROW_5);

//   ports[0] = digitalPinToPort(LED_ROW_1);
//   ports[1] = digitalPinToPort(LED_ROW_2);
//   ports[2] = digitalPinToPort(LED_ROW_3);
//   ports[3] = digitalPinToPort(LED_ROW_4);
//   ports[4] = digitalPinToPort(LED_ROW_5);

  // Set pins to output
  for(uint8_t i=0; i<CNT_PINS; ++i){
    pinMode(pins[i], OUTPUT);
    bits[i] = digitalPinToBitMask(pins[i]);
    ports[i] = digitalPinToPort(pins[i]);
  }

//   pinMode(12, OUTPUT);
//   digitalWrite(12, HIGH);

//   for(uint8_t i=0; i<5; ++i)
//     digitalWrite(pins[i], HIGH);
}

void Counter::off(){
  volatile uint8_t *out = portOutputRegister(ports[pos]);
  *out &= ~bits[pos];
//   digitalWrite(pins[pos], LOW);
}

void Counter::on(){
  volatile uint8_t *out = portOutputRegister(ports[pos]);
  *out |= bits[pos];
//   digitalWrite(pins[pos], HIGH);
}

void Counter::increment(){
  off();
  if(++pos == CNT_PINS)
    pos = 0;
  on();
}

