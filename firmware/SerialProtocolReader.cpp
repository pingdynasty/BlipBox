#include <wiring.h>
#include <avr/interrupt.h>
#include "globals.h"
#include "Greeting.h"
#include "Characters.h"
#include "Parameters.h"

#define RX_BUFFER_SIZE 3

#define CLEAR_MESSAGE             0x10

#define SET_LED_MESSAGE           0x20 // sets 1 led - 3 byte message
// set led: 4 bits marker type, 8 bits led index, 8 bits brightness
// 1000mmmm llllllll bbbbbbbb
#define SET_LED_ROW_MESSAGE       0x30 // sets 8 leds - two byte message
// led row: 0011cccc bbbbbbbb : 4 bit row index i, bit mask b
#define SET_LED_COL_MESSAGE       0x40 // sets 8 leds - two byte message
// led column: 0100iiii bbbbbbbb : 4 bit column index i, bit mask b

#define WRITE_CHARACTER_MESSAGE   0x50
#define SHIFT_LEDS_MESSAGE        0x60
#define COMMAND_MESSAGE           0x70
// command: 0111cccc : 4 bit command index

#define MESSAGE_ID_MASK           0xf0
#define MESSAGE_VALUE_MASK        0x0f

#define SET_PARAMETER_MESSAGE     0xc0
// set parameter: 11ppppvv vvvvvvvv : 4 bit parameter ID p, 10 bit value v
#define PARAMETER_ID_MASK         0x3c
#define PARAMETER_VALUE_MASK      0x03

unsigned char rx_buffer[RX_BUFFER_SIZE];
uint8_t rx_buffer_head = 0;

// #define MESSAGE_TYPE
uint8_t getMessageType(){
  return rx_buffer[0] & MESSAGE_ID_MASK;
}

/* extract 12 bits of data from the first two message bytes */
uint16_t getTwelveBitValue(){
  return ((rx_buffer[0] & MESSAGE_VALUE_MASK) << 8) | rx_buffer[1];
}

/* extract 10 bits of data from the first two message bytes */
uint16_t getTenBitValue(){
  return ((rx_buffer[0] & PARAMETER_VALUE_MASK) << 8) | rx_buffer[1];
}

// #define FOUR_BIT_VALUE rx_buffer[0] & MESSAGE_VALUE_MASK;
uint8_t getFourBitValue(){
  return rx_buffer[0] & MESSAGE_VALUE_MASK;
}

void handleSetLedMessage(){
    rx_buffer_head = 0;
  // set led: 4 bits marker type, 8 bits led index, 8 bits brightness
  // 1000mmmm llllllll bbbbbbbb
    blipbox.leds.setLed(rx_buffer[1] / 16, rx_buffer[1] % 16, rx_buffer[2]);
}

void handleSetLedRowMessage(){  
  rx_buffer_head = 0;
  for(uint8_t i=0; i<8; ++i)
    blipbox.leds.setLed(i, getFourBitValue(),
			(rx_buffer[1] & _BV(i)) ? blipbox.config.brightness : 0 );
}

void handleSetLedColumnMessage(){
  rx_buffer_head = 0;
  for(uint8_t i=0; i<8; ++i)
    blipbox.leds.setLed(getFourBitValue(), i, 
			(rx_buffer[1] & _BV(i)) ? blipbox.config.brightness : 0 );
}

void handleWriteCharacterMessage(){
  rx_buffer_head = 0;
  blipbox.display.printCharacter(rx_buffer[1], getFourBitValue(), 0, blipbox.config.brightness);
//   uint8_t data[getCharacterHeight()];
//   getCharacterData(rx_buffer[1], data);
//   blipbox.display.printCharacter(data, getFourBitValue(), 0, blipbox.config.brightness);
}

void handleSetParameterMessage(){
  rx_buffer_head = 0;
  setParameter(rx_buffer[0] & PARAMETER_ID_MASK, getTenBitValue());
}

void handleClearMessage(){
  rx_buffer_head = 0;
  blipbox.leds.fill(getFourBitValue() * 0x11);
}

void handleShiftLedsMessage(){
  rx_buffer_head = 0;
  blipbox.display.shift(getFourBitValue());
}

void handleCommandMessage(){
  rx_buffer_head = 0;
  switch(getFourBitValue()){
  case 3: // toggle / xor
    blipbox.leds.toggle();
    break;
  case 4: // fade
    blipbox.leds.sub(4);
    break;
  case 5: // brighten
    blipbox.leds.add(4);
    break;
  case 6: // 
    blipbox.sendConfigurationParameters();
    break;
  case 7: // reset configuration to defaults
    blipbox.config.reset();
    break;
  case 8: // read configuration from eeprom (if possible)
    blipbox.config.init();
    break;
  case 9: // write configuration to eeprom
    blipbox.config.write();
    break;
  case 10: // stop led update
    blipbox.leds.stop();
    break;
  case 11: // start led update
    blipbox.leds.start();
    break;
  case 15: // re-initialise
    blipbox.init();
    break;
  default:
    blipbox.message(MESSAGE_READ_ERROR);
    break;
  }
}

void serialInput(unsigned char c) {
  rx_buffer[rx_buffer_head++] = c;
  switch(getMessageType()){
    // 3 byte messages
  case SET_LED_MESSAGE:
    if(rx_buffer_head == 3)
      handleSetLedMessage();
    break;
    // 2 byte messages
  case SET_LED_ROW_MESSAGE:
    if(rx_buffer_head == 2)
      handleSetLedRowMessage();
    break;
  case SET_LED_COL_MESSAGE:
    if(rx_buffer_head == 2)
      handleSetLedColumnMessage();
    break;
  case WRITE_CHARACTER_MESSAGE:
    if(rx_buffer_head == 2)
      handleWriteCharacterMessage();
    break;
    // 1 byte messages
  case CLEAR_MESSAGE:
    handleClearMessage();
    break;
  case SHIFT_LEDS_MESSAGE:
    handleShiftLedsMessage();
    break;
  case COMMAND_MESSAGE:
    handleCommandMessage();
    break;
  default:
    // set parameter has a two-bit signature
    if((rx_buffer[0] & 0xc0) == SET_PARAMETER_MESSAGE){
      //   case SET_PARAMETER_MESSAGE:
      if(rx_buffer_head == 2)
	handleSetParameterMessage();
    }else{
      rx_buffer_head = 0;
      blipbox.message(MESSAGE_READ_ERROR);
    }
  }
}
