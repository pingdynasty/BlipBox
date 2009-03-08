#include <wiring.h>
#include "MonomeSeriesProtocol.h"
#include "defs.h"
#include "globals.h"
#include "Greeting.h"

void MonomeSeriesProtocol::init(){
  keys.init();
  leds.init();
  beginSerial(9600); // fixed for monome series
  setmode(EEPROM_MODE_BLIPBOX); // set the mode to use after next system reset

  // greeting
  rain(leds);
  fade(leds);
}

void MonomeSeriesProtocol::process(){
  if(serialAvailable() > 0)
    this->readMessage();
  if(keys.keyscan()){
    // note that KeyController grid rows/columns are our cols/rows!
    // invert values
    uint8_t col = 7 - keys.getRow();
    uint8_t row = 9 - keys.getColumn();
    this->keyUp(row, col); // release old key
    if(keys.isPressed()){
      keys.update(); // update keycontroller values
      col = 7 - keys.getRow();
      row = 9 - keys.getColumn();
      this->keyDown(row, col); // press new key
    }
  }
#ifdef FRAME_DELAY
  delay(FRAME_DELAY);
#endif
}

void MonomeSeriesProtocol::readMessage(){
  data[0] = readByte();
  switch(data[0]>>4) {
  case 2:
    // led on
    data[1] = readByte();
    led_on(data[1]>>4, data[1]&0x0f);
    break;
  case 3:
    // led off
    data[1] = readByte();
    led_off(data[1]>>4, data[1]&0x0f);
    break;
  case 4:
    // led row 1
    data[1] = readByte();
    led_col1(data[0]&0x0f, data[1]);
    break;
  case 5:
    // led col 1
    data[1] = readByte();
    led_row1(data[0]&0x0f, data[1]);
    break;
  case 6:
    // led row 2
    data[1] = readByte();
    data[2] = readByte();
    led_col2(data[0]&0x0f, data[1], data[2]);
    break;
  case 7:
    // led col 2
    data[1] = readByte();
    data[2] = readByte();
    led_row2(data[0]&0x0f, data[1], data[2]);
    break;
  case 8: {
    data[1] = readByte();
    data[2] = readByte();
    data[3] = readByte();
    data[4] = readByte();
    data[5] = readByte();
    data[6] = readByte();
    data[7] = readByte();
    data[8] = readByte();
    led_frame(data[0]&0x0f, data+1);
    break;
  }
  case 9:
    clear(data[0]&0x0f);
    break;
  case 10:
    intensity(data[0]&0x0f);
    break;
  case 11:
    mode(data[0]&0x0f);
    break;
  case 12:
    port_on(data[0]&0x0f);
    break;
  case 13:
    port_off(data[0]&0x0f);
    break;
  default:
    error(MESSAGE_READ_ERROR);
    // chomp up the rest of the garbled message
    serialFlush();
    break;
  }
  if(serialAvailable() > 121)
    serialFlush();
}

void MonomeSeriesProtocol::keyDown(uint8_t row, uint8_t col){
  serialWrite(0);
  serialWrite((row << 4) | (col & 0x0F)); 
}

void MonomeSeriesProtocol::keyUp(uint8_t row, uint8_t col){
  serialWrite(1 << 4);
  serialWrite((row << 4) | (col & 0x0F)); 
}

// void MonomeSeriesProtocol::sendAux(uint8_t port, uint8_t value){
//   Serial.print((14 << 4) | (port & 0x0F), BYTE);
//   Serial.print(value & 0xFF, BYTE);
// }

// uint8_t Message::data[MAX_MESSAGE_LENGTH];


// monome serial protocol

void MonomeSeriesProtocol::led_row1(MSP_ARG_TYPE row, MSP_ARG_TYPE data) {
  for(uint8_t col=0; col < 8; col++)
    if(data & _BV(col))
      led_on(row, col);
    else
      led_off(row, col);
}

void MonomeSeriesProtocol::led_col1(MSP_ARG_TYPE col, MSP_ARG_TYPE data) {
  for(uint8_t row=0; row < 8; row++)
    if(data & _BV(row))
      led_on(row, col);
    else
      led_off(row, col);
}

void MonomeSeriesProtocol::led_row2(MSP_ARG_TYPE row, MSP_ARG_TYPE rowa, MSP_ARG_TYPE rowb) {
  led_row1(row, rowa); // set first 8 leds
  for(uint8_t col=8; col<16; col++)
    if(rowb & _BV(col-8))
      led_on(row, col);
    else
      led_off(row, col);
}

void MonomeSeriesProtocol::led_col2(MSP_ARG_TYPE col, MSP_ARG_TYPE cola, MSP_ARG_TYPE colb) {
  led_col1(col, cola); // set first 8 leds
  for(uint8_t row=8; row<16; row++)
    if(colb & _BV(row-8))
      led_on(row, col);
    else
      led_off(row, col);
}

void MonomeSeriesProtocol::led_frame(MSP_ARG_TYPE quadrant, uint8_t* rows) {
//   quadrants are from top left to bottom right, as shown:
//   0 1
//   2 3
  uint8_t fromRow = quadrant > 1 ? 8 : 0;
  uint8_t fromCol = quadrant %2 == 1 ? 8 : 0;
  for(uint8_t i=0; i<8; ++i)
    for(uint8_t j=0; j<8; ++j)
      if(rows[i] & _BV(j))
        led_on(fromRow+i, fromCol+j);
      else
        led_off(fromRow+i, fromCol+j);
}

void MonomeSeriesProtocol::clear(MSP_ARG_TYPE state) {
  for(uint8_t row=0; row<16; row++)
    for(uint8_t col=0; col<16; col++)
      if(state == 0)
        led_off(row, col);
      else
        led_on(row, col);
// note:		clear state of 0 turns off all leds.
// 			clear state of 1 turns on all leds.
}

void MonomeSeriesProtocol::mode(MSP_ARG_TYPE mode) {
  switch (mode) {
  case 0:
    break;
  case 1:
    clear(1);
    break;
  case 2:
    clear(0);
    break;
  case 3:
    // special mode!
    // switch into blipbox mode
    // sets an eeprom flag
    setmode(EEPROM_MODE_BLIPBOX);
    // does not reset
    break;
  case 4:
    setmode(EEPROM_MODE_MONOME_SERIES);
    // does not reset
    break;
    // 4 bits available for mode
  default:
    error(MODE_ERROR);
    serialFlush();
  }
// note:        mode = 0 : normal
//              mode = 1 : test (all leds on)
//              mode = 2 : shutdown (all leds off)
}

void MonomeSeriesProtocol::intensity(MSP_ARG_TYPE level) {
  brightness = level;
}

void MonomeSeriesProtocol::port_on(MSP_ARG_TYPE port) {
  // TODO
}

void MonomeSeriesProtocol::port_off(MSP_ARG_TYPE port) {
  // TODO
}

void MonomeSeriesProtocol::led_on(MSP_ARG_TYPE row, MSP_ARG_TYPE col){
  if(row < 10 && col < 8)
    leds.setLed(row, col, brightness);
}

void MonomeSeriesProtocol::led_off(MSP_ARG_TYPE row, MSP_ARG_TYPE col){
  if(row < 10 && col < 8)
    leds.setLed(row, col, 0);
}

//   MSP_ARG_TYPE type = Serial.read();
//   MSP_ARG_TYPE val = type & 0x0f;
//   type >>= 4;
//   switch(type) {
//   case 2:
//     led_on(Serial.read());
//     break;
//   case 3:
//     led_off(Serial.read());
//     break;
//   case 4:
//     led_row1(val, Serial.read());
//     break;
//   case 5:
//     led_col1(val, Serial.read());
//     break;
//   case 6:
//     led_row2(val,Serial.read(), Serial.read());
//     break;
//   case 7:
//     led_col2(val,Serial.read(),Serial.read());
//     break;
//   case 8: {
//     MSP_ARG_TYPE rows[] = {Serial.read(), Serial.read(), Serial.read(), Serial.read(), 
//                       Serial.read(), Serial.read(), Serial.read(), Serial.read()};
//     led_frame(val, rows);
//     break;
//   }
//   case 9:
//     clear(val);
//     break;
//   case 10:
//     intensity(val);
//     break;
//   case 11:
//     mode(val);
//     break;
//   case 12:
//     port_on(val);
//     break;
//   case 13:
//     port_off(val);
//     break;
//   default:
//     break;
//   }
// }
