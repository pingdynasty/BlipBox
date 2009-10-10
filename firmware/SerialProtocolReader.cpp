#include <wiring.h>
#include <avr/interrupt.h>
#include "globals.h"
#include "Greeting.h"
#include "Characters.h"

#define RX_BUFFER_SIZE 3

#define CLEAR_MESSAGE             0x10

#define SET_LED_MESSAGE           0x20 // sets 1 led - 3 byte message
#define SET_LED_ROW_MESSAGE       0x30 // sets 8 leds - two byte message
#define SET_LED_COL_MESSAGE       0x40 // sets 8 leds - two byte message

#define WRITE_CHARACTER_MESSAGE   0x50
#define SHIFT_LEDS_MESSAGE        0x60
#define COMMAND_MESSAGE           0x70

#define SET_PARAMETER_MESSAGE     0xc0
// set parameter: 11ppppvv vvvvvvvv : 4 bit parameter ID p, 10 bit value v

#define MESSAGE_ID_MASK           0xf0
#define MESSAGE_VALUE_MASK        0x0f

#define PARAMETER_ID_MASK         0x3c
#define PARAMETER_VALUE_MASK      0x03

unsigned char rx_buffer[RX_BUFFER_SIZE];
uint8_t rx_buffer_head = 0;

// #define MESSAGE_TYPE
uint8_t getMessageType(){
  return rx_buffer[0] & MESSAGE_ID_MASK;
}

/* extract 12 bits of data from the first two message bytes as a 16 bit value */
uint16_t getTwelveBitValue(){
  return ((rx_buffer[0] & MESSAGE_VALUE_MASK) << 8) | rx_buffer[1];
}

/* extract 10 bits of data from the first two message bytes as a 16 bit value */
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
  leds.setLed(9 - rx_buffer[1] / 16, rx_buffer[1] % 16, rx_buffer[2]);
}

void handleSetLedRowMessage(){  
  rx_buffer_head = 0;
  for(uint8_t i=0; i<8; ++i)
    leds.setLed(i, getFourBitValue(),
                (rx_buffer[1] & _BV(i)) ? config.brightness : 0 );
}

void handleSetLedColumnMessage(){
  rx_buffer_head = 0;
  for(uint8_t i=0; i<8; ++i)
    leds.setLed(getFourBitValue(), i, 
                (rx_buffer[1] & _BV(i)) ? config.brightness : 0 );
}

void handleWriteCharacterMessage(){
  rx_buffer_head = 0;
  display.printCharacter(getCharacterData(rx_buffer[1]),
                         getFourBitValue(), 0, config.brightness);
}

#define BRIGHTNESS_PARAMETER_ID       (0x01 << 2)
#define SENSITIVITY_PARAMETER_ID      (0x02 << 2)

#define SERIAL_SPEED_PARAMETER_ID     (0x08 << 2)

void handleSetParameterMessage(){
  rx_buffer_head = 0;
  // todo! ignored for now
  switch(rx_buffer[0] & PARAMETER_ID_MASK){
  case SENSITIVITY_PARAMETER_ID:
    config.sensitivity = getTenBitValue();
    break;
  case BRIGHTNESS_PARAMETER_ID:
    config.brightness = getTenBitValue();
    break;
  case SERIAL_SPEED_PARAMETER_ID:
    config.serialSpeed = getTenBitValue();
    break;
    //       case FOLLOW_MODE_PARAMETER_ID:
    //         follow = receiver.getTenBitValue();
    //         break;
    //       case X_MIN_PARAMETER_ID:
    //         touchscreen_x_min = receiver.getTenBitValue();
    //         break;
    //       case Y_MIN_PARAMETER_ID:
    //         touchscreen_y_min = receiver.getTenBitValue();
    //         break;
    //       case X_RANGE_PARAMETER_ID:
    //         touchscreen_x_range = receiver.getTenBitValue();
    //         break;
    //       case Y_RANGE_PARAMETER_ID:
    //         touchscreen_y_range = receiver.getTenBitValue();
    //         break;
    //       }
    // //     }else{
    // //       error(MESSAGE_READ_ERROR);
    // //       serialFlush();
    //     }
    //   }
  }
}

void handleClearMessage(){
  rx_buffer_head = 0;
  leds.fill(getFourBitValue() * 0x11);
}

void handleShiftLedsMessage(){
  rx_buffer_head = 0;
  display.shift(getFourBitValue());
}

void handleCommandMessage(){
  rx_buffer_head = 0;
  switch(getFourBitValue()){
  case 5:
    leds.fade(1);
    break;
  case 6:
    leds.brighten(1);
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
  case SET_PARAMETER_MESSAGE:
    if(rx_buffer_head == 2)
      handleSetParameterMessage();
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
    rx_buffer_head = 0;
    error(MESSAGE_READ_ERROR);
  }
}
