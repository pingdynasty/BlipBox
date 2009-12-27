#include "MCP492xController.h"
#include "DDS.h"

#define FREQ_POT_PIN 0

MCP492xController dac;
DDS dds;

// #define DEBUG

void setup() { 
  Serial.begin(9600);
  Serial.println("hello");

  dac.init();
  dds.init();

  pinMode(7, OUTPUT);
 
  Serial.println("let's go");
}

uint8_t cnt;

void loop () {
  if(++cnt == 0){
    digitalWrite(7, LOW);
    dds.setFrequency(analogRead(FREQ_POT_PIN)<<2);
  }else if(cnt == 127){
    digitalWrite(7, HIGH);
  }

  uint16_t value = dds.getValue();
  dac.send(value<<4);

#ifdef DEBUG
  Serial.println(value, HEX);
  delay(5);
#endif
}
