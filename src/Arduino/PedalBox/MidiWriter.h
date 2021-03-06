#ifndef _MIDIWRITER_H_
#define _MIDIWRITER_H_

#include "MidiInterface.h"

class MidiWriter : public MidiInterface {
public:

  /* @channel 1-16 */
  void init(uint8_t _channel){
    channel = _channel-1;
  }

  void channelPressure(int value){
    write(0xd0 | channel);
    write(value & 0x7f);
  }

  void startSong(){
    write(0xfa);
  }

  void stopSong(){
    write(0xfc);
  }

 void continueSong(){
    write(0xfb);
  }

 void midiClock(){
   write(0xf8);
  }

  void pitchBend(int16_t value){
    write(0xe0 | channel);
    write(value & 0x7f);
    write((value>>7) & 0x7f);
  }

 void controlChange(int cc, int value){
   write(0xb0 | channel);
   write(cc & 0x7f);
   write(value & 0x7f);
 }

 void noteOff(int note, int velocity){
   write(0x80 | channel);
   write(note & 0x7f);
   write(velocity & 0x7f);
 }

 void noteOn(int note, int velocity){
   write(0x90 | channel);
   write(note & 0x7f);
   write(velocity & 0x7f);
 }

  void allNotesOff(){
    controlChange(0x7b, 00);
  }

private:
  uint8_t channel;

  void write(uint8_t data){
    serialWrite(data);
//     Serial.print(data);
  }

};


#endif /* _MIDIWRITER_H_ */
