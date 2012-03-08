#include "Protocol.h"
#include "SerialProtocolReader.h"
#include "globals.h"
#include "Greeting.h"
#include "Characters.h"
#include "Parameters.h"
#include "Command.h"
#include "PresetCommandReader.h"

// #define MESSAGE_TYPE
uint8_t SerialProtocolReader::getMessageType(){
  return rx_buffer[0] & MESSAGE_ID_MASK;
}

/* extract 12 bits of data from the first two message bytes */
uint16_t SerialProtocolReader::getTwelveBitValue(){
  return ((rx_buffer[0] & MESSAGE_VALUE_MASK) << 8) | rx_buffer[1];
}

/* extract 10 bits of data from the first two message bytes */
uint16_t SerialProtocolReader::getTenBitValue(){
  return ((rx_buffer[0] & PARAMETER_VALUE_MASK) << 8) | rx_buffer[1];
}

// #define FOUR_BIT_VALUE rx_buffer[0] & MESSAGE_VALUE_MASK;
uint8_t SerialProtocolReader::getFourBitValue(){
  return rx_buffer[0] & MESSAGE_VALUE_MASK;
}

void SerialProtocolReader::handleMidiPresetCommand(){
  blipbox.setSerialReader(new PresetCommandReader());
}

void SerialProtocolReader::handleSetLedMessage(){
  rx_buffer_head = 0;
  // set led: 4 bits marker type, 8 bits led index, 8 bits brightness
  // 1000mmmm llllllll bbbbbbbb
  blipbox.leds.setLed(rx_buffer[1] / 16, rx_buffer[1] % 16, rx_buffer[2]);
  if(autoflip)
    blipbox.leds.flip();
}

void SerialProtocolReader::handleSetLedRowMessage(){  
  rx_buffer_head = 0;
  for(uint8_t i=0; i<8; ++i)
    blipbox.leds.setLed(i, getFourBitValue(),
			(rx_buffer[1] & _BV(i)) ? blipbox.config.brightness : 0 );
  if(autoflip)
    blipbox.leds.flip();
}

void SerialProtocolReader::handleSetLedColumnMessage(){
  rx_buffer_head = 0;
  for(uint8_t i=0; i<8; ++i)
    blipbox.leds.setLed(getFourBitValue(), i, 
			(rx_buffer[1] & _BV(i)) ? blipbox.config.brightness : 0 );
  if(autoflip)
    blipbox.leds.flip();
}

void SerialProtocolReader::handleWriteCharacterMessage(){
  rx_buffer_head = 0;
  blipbox.display.printCharacter(rx_buffer[1], getFourBitValue(), 0, blipbox.config.brightness);
  if(autoflip)
    blipbox.leds.flip();
//   uint8_t data[getCharacterHeight()];
//   getCharacterData(rx_buffer[1], data);
//   blipbox.display.printCharacter(data, getFourBitValue(), 0, blipbox.config.brightness);
}

void SerialProtocolReader::handleSetParameterMessage(){
  rx_buffer_head = 0;
  setParameter(rx_buffer[0] & PARAMETER_ID_MASK, getTenBitValue());
}

void SerialProtocolReader::handleFillMessage(){
  rx_buffer_head = 0;
  blipbox.leds.fill(getFourBitValue() * 0x11);
  if(autoflip)
    blipbox.leds.flip();
}

void SerialProtocolReader::handleShiftLedsMessage(){
  rx_buffer_head = 0;
  blipbox.display.shift(getFourBitValue());
}

void SerialProtocolReader::handleCommandMessage(){
  rx_buffer_head = 0;
  switch(getFourBitValue()){
  case TOGGLE: // toggle / xor
    blipbox.leds.toggle();
    if(autoflip)
      blipbox.leds.flip();
    break;
  case FADE: // fade
    blipbox.leds.sub(4);
    if(autoflip)
      blipbox.leds.flip();
    break;
  case BRIGHTEN: // brighten
    blipbox.leds.add(4);
    if(autoflip)
      blipbox.leds.flip();
    break;
  case CFG_REQUEST: // 
    blipbox.sendConfigurationParameters();
    break;
  case CFG_RESET: // reset configuration to defaults
    blipbox.config.reset();
    break;
  case CFG_READ: // read configuration from eeprom (if possible)
    blipbox.config.init();
    break;
  case CFG_WRITE: // write configuration to eeprom
    blipbox.config.write();
    break;
  case START_LED_BLOCK: // start LED command block
    autoflip = false;
    break;    
  case END_LED_BLOCK: // end LED command block
    autoflip = true;
    blipbox.leds.flip();
    break;    
//   case ENABLE_MENU:
//     break;    
//   case DISABLE_MENU:
//     break;    
  case MIDI_PRESET: // receive midi preset command
    handleMidiPresetCommand();
    break;
  case RESET: // re-initialise
    blipbox.init();
    break;
  default:
    blipbox.message(MESSAGE_READ_ERROR);
    break;
  }
}

void SerialProtocolReader::serialInput(unsigned char c) {
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
  case FILL_MESSAGE:
    handleFillMessage();
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
