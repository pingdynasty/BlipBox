#include "MidiMessageReader.h"
#include "globals.h"
#include "MidiConstants.h"
#include "Event.h"
// #include <string.h>

MidiMessageReader::MidiMessageReader() 
  : pos(0) {
//   memset(buf, 0, sizeof(buf));
}

void MidiMessageReader::serialInput(unsigned char c){
  if(pos == 0){
    buf[pos++] = c;
    switch(c & MIDI_STATUS_MASK){
    case MIDI_NOTE_OFF:
    case MIDI_NOTE_ON:
    case MIDI_CONTROL_CHANGE:
    case MIDI_AFTERTOUCH:
    case MIDI_PITCH_BEND:
      len = 3;
      break;
    case MIDI_PROGRAM_CHANGE:
    case MIDI_CHANNEL_PRESSURE:
      len = 2;
      break;
    case MIDI_SYSTEM_MESSAGE:
      len = 1;
      if(c == MIDI_SYSEX_START)
	// ignore sysex messages
	pos = 0;
      break;
    default:
      pos = 0;
    }
  }else{
    buf[pos++] = c;
    if(pos == len){
      pos = 0;
//       memset(buf, 0, sizeof(buf));
      MidiEvent event(buf[0], buf[1], buf[2]);
      blipbox.eventhandler->handle(event);
    }
  }
}
