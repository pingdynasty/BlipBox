#ifndef _CONSOLEREADER_H_
#define _CONSOLEREADER_H_

#include <wiring.h>

#include "MidiCvInterface.h"

class ConsoleReader {
public:

  void init(MidiCvInterface* _midi){
    midi = _midi;
    Serial.begin(9600);
    Serial.println("hello");
  }

  void read(){
    char type = read_console_char();
    uint8_t data = read_console_hex() * 16;
    data += read_console_hex();
    switch(type){
    case '1':
      midi->cv1 = data*16+read_console_hex();
      break;
    case 'f':
      midi->factor = data;
      break;
    case '2':
      midi->setCV(2, data);
      break;
    case '3':
      midi->setCV(3, data);
      break;
    case 'c':
      midi->controlChange(data, read_console_hex()*16+read_console_hex());
      break;
    case 'p':
      midi->pitchBend(data*16+read_console_hex());
      break;
    case 'n':
      midi->noteOn(data, 0x7f);
      break;
    case 'o':
      midi->noteOff(data, 0x00);
    }
    Serial.print(type, BYTE);
    Serial.print(": 0x");
    Serial.println(data, HEX);
    Serial.print("cv1: 0x");
    Serial.print(midi->cv1, HEX);
    Serial.print("\tcv2: 0x");
    Serial.print(midi->getCV(2), HEX);
    Serial.print("\tcv3: 0x");
    Serial.print(midi->getCV(3), HEX);
    Serial.print("\tfac: 0x");
    Serial.println(midi->factor, HEX);
  }

private:
  MidiCvInterface* midi;

  byte read_console_char(){
    while(!Serial.available());
    return Serial.read();
  }

  int read_console_hex(){
    byte b = read_console_char();
    if(b > 0x30 && b < 0x3a) // 0=0x30, 9=0x39
      return b-0x30;
    if(b > 0x60 && b < 0x67) // a=0x61, f=0x66
      return b-0x57;
    return 0;
  }

};


#endif /* _CONSOLEREADER_H_ */
