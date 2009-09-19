#include "BlipBoxProtocol.h"

#include <wiring.h>
// #include <avr/eeprom.h>

// #include "SerialInterrupt.h"
#include "defs.h"
#include "device.h"
#include "globals.h"
#include "Greeting.h"
#include "MessageSender.h"
// #include "MessageReceiver.h"
#include "Characters.h"
#include "Parameters.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/delay.h>
#include "globals.h"
#include "Characters.h"

MessageSender sender;
// MessageReceiver receiver;

unsigned long previousMillis = 0;        // will store last time write was done

#define NO_FOLLOW_MODE 0x00
#define DOT_MODE       0x01
#define CROSS_MODE     0x02
#define CRISS_MODE     0x03
#define STAR_MODE      0x04
#define BLOB_MODE      0x05
#define SQUARE_MODE    0x06

uint8_t follow; // default to 0: NO_FOLLOW_MODE
uint8_t holding;
uint8_t holdCol, holdRow;
uint16_t sensitivity = SENSITIVITY; // todo: read/write to eeprom
uint8_t brightness;
Animator* animator;
// FadeAnimator fader(10);

void BlipBoxProtocol::init(){

  // initialise subsystems
  keys.init();
  leds.init();
  sender.init();

//   greeting(leds);

  // set serial speed
//   uint16_t val = getParameter(SERIAL_SPEED_PARAMETER_ID);
//   if(val == 0 || val > 115200/9600)
    beginSerial(DEFAULT_SERIAL_SPEED);
//   else
//     beginSerial(val * 9600l);
  // 2: 19200
  // 4: 38400
  // 6: 57600
  // 12: 115200

//   brightness = getParameter(BRIGHTNESS_PARAMETER_ID);
//   if(brightness == 0)
    brightness = BRIGHTNESS;
  // todo! load these from eeprom
// uint16_t touchscreen_x_min   = 160;
// uint16_t touchscreen_x_range = 860 - touchscreen_x_min;
// uint16_t touchscreen_y_min   = 160;
// uint16_t touchscreen_y_range = 860 - touchscreen_y_min;
}

void BlipBoxProtocol::process(){
  uint8_t col, row;

//   if(keys.keyscan() && follow){
//     /* key position has changed */
//     /* if follow mode = none, then the leds aren't cleared so that leds stay lit */
//     leds.clear();
//     if(keys.isPressed()){
//       // note: inversed and swapped values!
//       col = 7 - keys.getRow();
//       row = keys.getColumn();
//       switch(follow){
//       case CROSS_MODE:
//         leds.setCross(row, col, brightness);
//         break;
//       case CRISS_MODE:
//         leds.setDiagonalCross(row, col, brightness);
//         break;
//       case DOT_MODE:
//         leds.setLed(row, col, brightness);
//         break;
//       case STAR_MODE:
//         leds.setStar(row, col, brightness);
//         break;
//       case BLOB_MODE:
//         leds.setBlob(row, col, brightness);
//         break;
//       case SQUARE_MODE:
//         leds.setSquare(row, col, brightness);
//         break;
//       }
//     }
//   }

//   if(holding)
//     leds.setLed(holdRow, holdCol, holding);

//   if(receiver.receiveMessage())
//     this->readMessage();

//   keys.keyscan();

  if(keys.getTouch() < sensitivity){
    // inverse X value
    sender.updateXY(SENSOR_MAX - keys.getX(), keys.getY(), keys.getTouch());
  }else{
    sender.updateRelease();
  }

//   this->readSensors();

  if(millis() - previousMillis > SERIAL_WRITE_INTERVAL){
    sender.sendNextMessage();
    previousMillis = millis();   // remember the last time we did this

    // counter overflows automatically from 255 back to 0
    signal.tick(counter++);
    if(animator)
      animator->tick(counter);
  }

}

// void BlipBoxProtocol::readSensors(){

//   uint16_t value;
//   value = analogRead(POT_PIN);
//   value &= (0xff << 2); // low level filter... todo: put a 10nF cap to ground at pot
//   sender.updateSensor(POT_SENSOR, value);

//   value = digitalRead(BUTTON1_PIN) == HIGH ? SENSOR_MAX : 0;

//   sender.updateSensor(BUTTON1_SENSOR, value);

//   if(sender.updateSensor(BUTTON1_SENSOR, value) && value){
//     // button has just been pressed
//     if(keys.isPressed()){
//       holding = brightness >> 2;
//       holdCol = 9 - keys.getColumn();
//       holdRow = 7 - keys.getRow();
//     }else{
//       holding = 0;
//     }
//     if(follow == CROSS_MODE){
//       follow = CRISS_MODE;
//     }else if(follow == CRISS_MODE){
//       follow = CROSS_MODE;
//     }
//     leds.clear(); // otherwise the hold point gets stuck
//   }

// }

// void BlipBoxProtocol::readMessage(){
//   switch(receiver.getMessageType()){
//   case CLEAR_MESSAGE:
//     leds.clear();
//     holding = 0;
//     break;
//   case DISPLAY_EFFECT_MESSAGE:
// //     displayEffect(receiver.getFourBitValue());
//     if(receiver.getFourBitValue() == 5)
//       leds.fade(1);
//     break;
//   case SET_LED_MESSAGE:
//     if(follow){
//       holdRow = 9 - receiver.getMessageData()[1] / 16;
//       holdCol = receiver.getMessageData()[1] % 16;
//       holding = receiver.getMessageData()[2];
//     }else{
//       leds.setLed(9 - receiver.getMessageData()[1] / 16, 
//                   receiver.getMessageData()[1] % 16, 
//                   receiver.getMessageData()[2]);
//     }
//     break;
//   case SET_LED_ROW_MESSAGE:
//     for(uint8_t i=0; i<8; ++i)
//       leds.setLed(i, receiver.getFourBitValue(),
//                   (receiver.getMessageData()[1] & _BV(i)) ? brightness : 0 );
//     break;
//   case SET_LED_COL_MESSAGE:
//     for(uint8_t i=0; i<8; ++i)
//       leds.setLed(receiver.getFourBitValue(), i, 
//                   (receiver.getMessageData()[1] & _BV(i)) ? brightness : 0 );
//     break;
//   case WRITE_CHARACTER_MESSAGE:
//     leds.printCharacter(getCharacterData(receiver.getMessageData()[1]),
//                         9 - receiver.getFourBitValue(), 0, brightness);
//     break;
//   case SHIFT_LEDS_MESSAGE:
//     leds.shift(receiver.getFourBitValue());
//     break;
//   default:
//     if((receiver.getMessageData()[0] & SET_PARAMETER_MESSAGE) == SET_PARAMETER_MESSAGE){
//       switch(receiver.getMessageData()[0] & PARAMETER_ID_MASK){
//       case SENSITIVITY_PARAMETER_ID:
//         setSensitivity(receiver.getTenBitValue());
//         break;
//       case BRIGHTNESS_PARAMETER_ID:
//         setBrightness(receiver.getTenBitValue());
//         break;
//       case SERIAL_SPEED_PARAMETER_ID:
//         setParameter(SERIAL_SPEED_PARAMETER_ID, receiver.getTenBitValue());
//         break;
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
// }

// void BlipBoxProtocol::setSensitivity(uint16_t value){
//   sensitivity = value;
//   setParameter(SENSITIVITY_PARAMETER_ID, value);
// }

// void BlipBoxProtocol::setBrightness(uint16_t value){
//   brightness = value;
//   setParameter(BRIGHTNESS_PARAMETER_ID, value);
// }




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
// uint8_t brightness = 0xff;

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
                (rx_buffer[1] & _BV(i)) ? brightness : 0 );
}

void handleSetLedColumnMessage(){
  rx_buffer_head = 0;
  for(uint8_t i=0; i<8; ++i)
    leds.setLed(getFourBitValue(), i, 
                (rx_buffer[1] & _BV(i)) ? brightness : 0 );
}

void handleWriteCharacterMessage(){
  rx_buffer_head = 0;
  leds.printCharacter(getCharacterData(rx_buffer[1]),
                      getFourBitValue(), 0, brightness);
}

void handleSetParameterMessage(){
  rx_buffer_head = 0;
  // todo! ignored for now
}

void handleClearMessage(){
  rx_buffer_head = 0;
  leds.fill(getFourBitValue() * 0x11);
}

void handleShiftLedsMessage(){
  rx_buffer_head = 0;
  leds.shift(getFourBitValue());
}

void handleCommandMessage(){
  rx_buffer_head = 0;
  displayEffect(getFourBitValue());
}


#if defined(__AVR_ATmega168__)
SIGNAL(SIG_USART_RECV)
#else
SIGNAL(SIG_UART_RECV)
#endif
{
#if defined(__AVR_ATmega168__)
  unsigned char c = UDR0;
#else
  unsigned char c = UDR;
#endif
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
