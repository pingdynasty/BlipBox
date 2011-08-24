#include "../TouchController.h"
#include "../globals.h"

#define VALUE_COUNT 3
uint16_t adc_acc[VALUE_COUNT];
uint16_t adc_values[VALUE_COUNT];
uint8_t adc_mode;

void TouchController::init(){
  memset(adc_acc, 0, sizeof(adc_acc));
  memset(adc_values, 0, sizeof(adc_values));
  setValue(0, 1023);
}

void TouchController::setValue(uint8_t index, uint16_t value){
  adc_values[index] = value;
}

uint16_t TouchController::getValue(uint8_t index){
  return adc_values[index];
}

uint16_t TouchController::getZ(){
  return getValue(0);
}
