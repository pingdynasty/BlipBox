#ifndef _CONSOLEREADER_H_
#define _CONSOLEREADER_H_

#include <wiring.h>

#include "MidiInterface.h"

class ConsoleReader {
public:

  void init(MidiInterface* _midi){
    midi = _midi;
/*     beginSerial(9600); */
/*     printString("hello"); */
/*     printString("welcome to midi console"); */
    Serial.begin(9600);
    Serial.println("hello");
    Serial.println("welcome to midi console");
  }

  void read(){
    char type = read_console_char();
    uint8_t data = read_console_hex()*16 + read_console_hex();
/*     data += read_console_hex(); */
    switch(type){
    case 'c':
      midi->controlChange(data, read_console_hex()*16+read_console_hex());
      break;
    case 'p':
      midi->pitchBend((data*16+read_console_hex())*16+read_console_hex());
      break;
    case 'n':
      midi->noteOn(data, 0x7f);
      break;
    case 'o':
      midi->noteOff(data, 0x00);
      break;
    case '>':
      midi->startSong();
      break;
    case '<':
      midi->stopSong();
      break;
    case '!':
      midi->continueSong();
      break;
    case 't':
      while(--data)
        midi->midiClock();
      break;
    }
    Serial.print(type, BYTE);
    Serial.print(": 0x");
    Serial.println(data, HEX);
  }

private:
  MidiInterface* midi;

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
