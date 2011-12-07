#ifndef _MIDIREADER_H_
#define _MIDIREADER_H_

#include <inttypes.h>
#include "MidiInterface.h"
#include "MidiConstants.h"

#define MIDI_READER_STANDBY 0

class MidiReader {
private:
  MidiInterface* midi;
public:
  int8_t status, data1, data2;

public:
//   MidiReader(MidiInterface* mid) : 
//     midi(mid) {
//     reset();
//   }
  MidiReader(MidiInterface* mid) : 
    midi(mid), status(MIDI_READER_STANDBY), data1(-1), data2(-1) {}

/*   void init(MidiInterface* _midi){ */
/*     midi = _midi; */
/*     reset(); */
/*   } */

  void read(char incomingByte){
    switch(status & MIDI_STATUS_MASK){
    case MIDI_NOTE_ON:
      if(data1 == -1){
        data1 = incomingByte;
      }else if(data2 == -1){
        data2 = incomingByte;
        midi->noteOn(data1, data2);
        reset();
      }else{
        reset();
      }
      break;
    case MIDI_NOTE_OFF:
      if(data1 == -1){
        data1 = incomingByte;
      }else if(data2 == -1){
        data2 = incomingByte;
        midi->noteOff(data1, data2);
        reset();
      }else{
        reset();
      }
      break;
    case MIDI_CONTROL_CHANGE:
      if(data1 == -1){
        data1 = incomingByte;
      }else if(data2 == -1){
        data2 = incomingByte;
        midi->controlChange(data1, data2);
        if(data1 == 0x7b)
          midi->allNotesOff();
        reset();
      }else{
        reset();
      }
      break;
    case MIDI_PITCH_BEND:
      if(data1 == -1){
        data1 = incomingByte;
      }else if(data2 == -1){
        data2 = incomingByte;
        midi->pitchBend((data2 << 7) | data1);
        reset();
      }else{
        reset();
      }
      break;
    case MIDI_CHANNEL_PRESSURE:
      if(data1 == -1){
        data1 = incomingByte;
        midi->channelPressure(data1);
        reset();
      }else{
        reset();
      }
      break;
    case MIDI_READER_STANDBY:
    default:
      if((incomingByte & MIDI_STATUS_MASK) == 0xf0){
	switch(incomingByte){
	case MIDI_TIMING_CLOCK:
	  midi->midiClock();
	  break;
	case MIDI_TICK:
	  // A MIDI Tick message is sent at regular intervals of one message every 10 milliseconds. 
	  midi->midiTick();
	  break;
	case MIDI_START_SONG:
	  midi->startSong();
	  break;
	case MIDI_CONTINUE_SONG:
	  midi->continueSong();
	  break;
	case MIDI_STOP_SONG:
	  midi->stopSong();
	  break;
	}
      }else if(incomingByte & 0x80){
	status = incomingByte;
/*       }else{ */
/* 	status = MIDI_READER_STANDBY; */
      }
    }
  }

  void reset(){
    data1 = -1;
    data2 = -1;
    status = MIDI_READER_STANDBY;
  }

};

#endif /* _MIDIREADER_H_ */
