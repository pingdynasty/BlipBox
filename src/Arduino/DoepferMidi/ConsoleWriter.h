#ifndef _CONSOLEWRITER_H_
#define _CONSOLEWRITER_H_

class ConsoleWriter : public MidiInterface {
public:

  /* @channel 1-16 */
  void init(uint8_t _channel){
    channel = _channel-1;
  }

  void channelPressure(int value){
    Serial.print("channel pressure ");
    write(0xd0 | channel);
    write(value & 0x7f);
    Serial.println();
  }

  void startSong(){
    Serial.println("start");
  }

  void stopSong(){
   Serial.println("stop");
  }

 void continueSong(){
   Serial.println("continue");
  }

 void midiClock(){
   Serial.println("clock");
 }

  void pitchBend(int16_t value){
    Serial.print("pb ");
    write(0xe0 | channel);
    write(value & 0x7f);
    write((value>>7) & 0x7f);
    Serial.println();
  }

 void controlChange(int cc, int value){
    Serial.print("cc ");
   write(0xb0 | channel);
   write(cc & 0x7f);
   write(value & 0x7f);
    Serial.println();
 }

 void noteOff(int note, int velocity){
    Serial.print("note off ");
   write(0x90 | channel);
   write(note & 0x7f);
   write(velocity & 0x7f);
   Serial.println();
 }

 void noteOn(int note, int velocity){
    Serial.print("note on ");
    write(0x80 | channel);
    write(note & 0x7f);
    write(velocity & 0x7f);
    Serial.println();
 }

  void allNotesOff(){
    Serial.println("all notes off");
    controlChange(0x7b, 00);
  }

private:
  uint8_t channel;

  void write(uint8_t data){
    Serial.print(data, HEX);
  }
};


#endif /* _CONSOLEWRITER_H_ */
